#include "engine/objects/SpringsSystem.h"

#include <cstdio>

#include "engine/math/functions.h"

namespace engine::objects {

SpringsSystem::SpringsSystem(PointsSystem* points_system) : m_points_system(points_system), m_size(0), m_capacity(0) {}

uint32_t SpringsSystem::addSpring(uint32_t start_point, uint32_t end_point, float stiffness, float damping,
                                  float rest_length, float min_length, float max_length) {
    if (start_point == end_point)
        return -1;

    const size_t points_count = m_points_system->size();
    if (start_point >= points_count || end_point >= points_count)
        return -1;

    const uint32_t* is_active = m_points_system->isActive();
    if (!is_active[start_point] || !is_active[end_point])
        return -1;

    if (stiffness <= 0.0f || damping < 0.0f)
        return (-1);

    if (rest_length < 0.0f) {
        const float* pos_x = m_points_system->positionsX();
        const float* pos_y = m_points_system->positionsY();
        rest_length = math::lengthFromPos(pos_x[start_point], pos_x[end_point], pos_y[start_point], pos_y[end_point]);
    }
    if (rest_length < 1e-5f)
        rest_length = 1e-5f;

    if (min_length >= 0.0f) {
        if (rest_length < min_length)
            return -1;

        if (max_length >= 0.0f) {
            if (min_length > max_length || rest_length > max_length)
                return -1;
        } else {
            max_length = rest_length * 2;
        }
    } else {
        min_length = rest_length / 2;
        if (max_length < 0.0f) {
            max_length = rest_length * 2;
        } else if (rest_length > max_length) {
            return -1;
        }
    }
    min_length = std::max(min_length, 1e-5f);

    while (!m_free_indices.empty()) {
        size_t idx = m_free_indices.back();
        m_free_indices.pop_back();

        if (m_is_active[idx])
            continue;

        m_start_points[idx] = start_point;
        m_end_points[idx]   = end_point;
        m_damping[idx]      = damping;
        m_min_length[idx]   = min_length;
        m_max_length[idx]   = max_length;
        m_rest_length[idx]  = rest_length;
        m_stiffness[idx]    = stiffness;
        m_is_active[idx]    = true;
        return idx;
    }

    if (m_size >= m_capacity)
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);

    m_start_points.emplace_back(start_point);
    m_end_points.emplace_back(end_point);
    m_damping.emplace_back(damping);
    m_min_length.emplace_back(min_length);
    m_max_length.emplace_back(max_length);
    m_rest_length.emplace_back(rest_length);
    m_stiffness.emplace_back(stiffness);
    m_is_active.emplace_back(true);
    return m_size++;
}

void SpringsSystem::removeSpring(uint32_t idx) noexcept {
    if (idx >= m_size || !m_is_active[idx])
        return;

    m_is_active[idx] = false;
    m_free_indices.emplace_back(idx);
}

void SpringsSystem::clear() noexcept {
    m_start_points.clear();
    m_end_points.clear();
    m_damping.clear();
    m_min_length.clear();
    m_max_length.clear();
    m_rest_length.clear();
    m_stiffness.clear();
    m_is_active.clear();
    m_free_indices.clear();
    m_size = 0;
}

void SpringsSystem::reserve(size_t new_capacity) noexcept {
    m_start_points.reserve(new_capacity);
    m_end_points.reserve(new_capacity);
    m_damping.reserve(new_capacity);
    m_min_length.reserve(new_capacity);
    m_max_length.reserve(new_capacity);
    m_rest_length.reserve(new_capacity);
    m_stiffness.reserve(new_capacity);
    m_is_active.reserve(new_capacity);

    m_capacity = new_capacity;
}

void SpringsSystem::applySpringsForce(float dt) noexcept {
    constexpr int SIMD_WIDTH = 8;
    const size_t SIMD_ITERS  = m_size / SIMD_WIDTH;
    const size_t SIMD_REM    = m_size % SIMD_WIDTH;

    const float* pos_x      = m_points_system->positionsX();
    const float* pos_y      = m_points_system->positionsY();
    const float* prev_pos_x = m_points_system->prevPositionsX();
    const float* prev_pos_y = m_points_system->prevPositionsY();
    const float* inv_mass   = m_points_system->invertedMasses();
    float* acc_x            = m_points_system->accelerationsX();
    float* acc_y            = m_points_system->accelerationsY();

    const __m256 dt_v     = _mm256_set1_ps(dt);
    const __m256 inv_dt_v = _mm256_set1_ps(1.0f / dt);
    const __m256 one_v    = _mm256_set1_ps(1.0f);

    for (size_t i = 0; i < SIMD_ITERS; ++i) {
        const size_t base = SIMD_WIDTH * i;

        __m256i start_idx = _mm256_load_si256(reinterpret_cast<const __m256i*>(m_start_points.data() + base));
        __m256i end_idx   = _mm256_load_si256(reinterpret_cast<const __m256i*>(m_end_points.data() + base));

        __m256 k        = _mm256_load_ps(m_stiffness.data() + base);
        __m256 rest_len = _mm256_load_ps(m_rest_length.data() + base);
        __m256 damping  = _mm256_load_ps(m_damping.data() + base);

        // Load positions and velocities
        __m256 x1 = _mm256_i32gather_ps(pos_x, start_idx, sizeof(float));
        __m256 y1 = _mm256_i32gather_ps(pos_y, start_idx, sizeof(float));
        __m256 x2 = _mm256_i32gather_ps(pos_x, end_idx, sizeof(float));
        __m256 y2 = _mm256_i32gather_ps(pos_y, end_idx, sizeof(float));

        __m256 dx  = _mm256_sub_ps(x2, x1);
        __m256 dy  = _mm256_sub_ps(y2, y1);
        __m256 len = math::avxLengthFromD(dx, dy);

        // Spring force
        __m256 f_spring = _mm256_mul_ps(_mm256_sub_ps(len, rest_len), k);

        // Damping force
        __m256 inv_len = _mm256_div_ps(one_v, len);
        __m256 dir_x   = _mm256_mul_ps(dx, inv_len);
        __m256 dir_y   = _mm256_mul_ps(dy, inv_len);

        __m256 v1x =
            _mm256_mul_ps(_mm256_sub_ps(x1, _mm256_i32gather_ps(prev_pos_x, start_idx, sizeof(float))), inv_dt_v);
        __m256 v1y =
            _mm256_mul_ps(_mm256_sub_ps(y1, _mm256_i32gather_ps(prev_pos_y, start_idx, sizeof(float))), inv_dt_v);
        __m256 v2x =
            _mm256_mul_ps(_mm256_sub_ps(x2, _mm256_i32gather_ps(prev_pos_x, end_idx, sizeof(float))), inv_dt_v);
        __m256 v2y =
            _mm256_mul_ps(_mm256_sub_ps(y2, _mm256_i32gather_ps(prev_pos_y, end_idx, sizeof(float))), inv_dt_v);

        __m256 rel_v_proj =
            _mm256_add_ps(_mm256_mul_ps(_mm256_sub_ps(v2x, v1x), dir_x), _mm256_mul_ps(_mm256_sub_ps(v2y, v1y), dir_y));
        __m256 f_damp = _mm256_mul_ps(damping, rel_v_proj);

        // Combined force
        __m256 f_total = _mm256_add_ps(f_spring, f_damp);
        __m256 fx      = _mm256_mul_ps(f_total, dir_x);
        __m256 fy      = _mm256_mul_ps(f_total, dir_y);

        // Apply forces
        __m256 inv_m1 = _mm256_i32gather_ps(inv_mass, start_idx, sizeof(float));
        __m256 inv_m2 = _mm256_i32gather_ps(inv_mass, end_idx, sizeof(float));

        scatterData(acc_x, start_idx, _mm256_fmadd_ps(fx, inv_m1, _mm256_i32gather_ps(acc_x, start_idx, sizeof(float))),
                    sizeof(float));
        scatterData(acc_y, start_idx, _mm256_fmadd_ps(fy, inv_m1, _mm256_i32gather_ps(acc_y, start_idx, sizeof(float))),
                    sizeof(float));
        scatterData(acc_x, end_idx, _mm256_fnmadd_ps(fx, inv_m2, _mm256_i32gather_ps(acc_x, end_idx, sizeof(float))),
                    sizeof(float));
        scatterData(acc_y, end_idx, _mm256_fnmadd_ps(fy, inv_m2, _mm256_i32gather_ps(acc_y, end_idx, sizeof(float))),
                    sizeof(float));
    }

    // tail
    for (size_t i = SIMD_ITERS * SIMD_WIDTH; i < m_size; ++i) {
        const int start_idx = m_start_points[i];
        const int end_idx   = m_end_points[i];

        const float k        = m_stiffness[i];
        const float rest_len = m_rest_length[i];
        const float damping  = m_damping[i];

        // Load positions
        const float x1 = pos_x[start_idx];
        const float y1 = pos_y[start_idx];
        const float x2 = pos_x[end_idx];
        const float y2 = pos_y[end_idx];

        const float dx  = x2 - x1;
        const float dy  = y2 - y1;
        const float len = math::lengthFromD(dx, dy);

        // Spring force
        const float f_spring = (len - rest_len) * k;

        // Damping force
        const float inv_len = 1.0f / len;
        const float dir_x   = dx * inv_len;
        const float dir_y   = dy * inv_len;

        const float v1x = (x1 - prev_pos_x[start_idx]) / dt;
        const float v1y = (y1 - prev_pos_y[start_idx]) / dt;
        const float v2x = (x2 - prev_pos_x[end_idx]) / dt;
        const float v2y = (y2 - prev_pos_y[end_idx]) / dt;

        const float rel_v_proj = (v2x - v1x) * dir_x + (v2y - v1y) * dir_y;
        const float f_damp     = damping * rel_v_proj;

        // Combined force
        const float f_total = f_spring + f_damp;
        const float fx      = f_total * dir_x;
        const float fy      = f_total * dir_y;

        // Apply forces
        const float inv_m1 = inv_mass[start_idx];
        const float inv_m2 = inv_mass[end_idx];

        acc_x[start_idx] += fx * inv_m1;
        acc_y[start_idx] += fy * inv_m1;
        acc_x[end_idx] -= fx * inv_m2;
        acc_y[end_idx] -= fy * inv_m2;
    }
}

}  // namespace engine::objects