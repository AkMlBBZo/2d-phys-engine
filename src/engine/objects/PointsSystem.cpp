#include "engine/objects/PointsSystem.h"

namespace engine::objects {

PointsSystem::PointsSystem() : m_size(0), m_capacity(0) {}

uint32_t PointsSystem::addPoint(float x, float y, float mass) {
    if (m_size >= m_capacity)
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    while (!m_free_indices.empty()) {
        size_t idx = m_free_indices.back();
        m_free_indices.pop_back();

        if (m_is_active[idx])
            continue;

        m_pos_x[idx]         = x;
        m_pos_y[idx]         = y;
        m_prev_pos_x[idx]    = x;
        m_prev_pos_y[idx]    = y;
        m_mass[idx]          = mass;
        m_inverted_mass[idx] = mass == 0.0f ? 0.0f : 1.0f / mass;
        m_is_active[idx]     = true;
        return idx;
    }

    m_pos_x.emplace_back(x);
    m_pos_y.emplace_back(y);
    m_prev_pos_x.emplace_back(x);
    m_prev_pos_y.emplace_back(y);
    m_acceleration_x.emplace_back(0.0f);
    m_acceleration_y.emplace_back(0.0f);
    m_mass.emplace_back(mass);
    m_inverted_mass.emplace_back(mass == 0.0f ? 0.0f : 1.0f / mass);
    m_is_active.emplace_back(true);

    return m_size++;
}

void PointsSystem::removePoint(size_t idx) noexcept {
    if (idx >= m_size || !m_is_active[idx])
        return;

    m_mass[idx]          = 0.0f;
    m_inverted_mass[idx] = 0.0f;
    m_free_indices.push_back(idx);
}

void PointsSystem::clear() noexcept {
    m_pos_x.clear();
    m_pos_y.clear();
    m_prev_pos_x.clear();
    m_prev_pos_y.clear();
    m_acceleration_x.clear();
    m_acceleration_y.clear();
    m_mass.clear();
    m_inverted_mass.clear();
    m_free_indices.clear();
    m_is_active.clear();

    m_size = 0;
}

void PointsSystem::reserve(size_t new_capacity) noexcept {
    m_pos_x.reserve(new_capacity);
    m_pos_y.reserve(new_capacity);
    m_prev_pos_x.reserve(new_capacity);
    m_prev_pos_y.reserve(new_capacity);
    m_acceleration_x.reserve(new_capacity);
    m_acceleration_y.reserve(new_capacity);
    m_mass.reserve(new_capacity);
    m_inverted_mass.reserve(new_capacity);
    m_is_active.reserve(new_capacity);

    m_capacity = new_capacity;
}

void PointsSystem::applyForce(size_t idx, float force_x, float force_y) noexcept {
    if (idx >= m_size || !m_is_active[idx])
        return;

    m_acceleration_x[idx] += force_x * m_inverted_mass[idx];
    m_acceleration_y[idx] += force_y * m_inverted_mass[idx];
}

void PointsSystem::applyGlobalForce(float force_x, float force_y) noexcept {
    const int SIMD_WIDTH    = 8;
    const size_t SIMD_ITERS = m_size / SIMD_WIDTH;
    const size_t SIMD_REM   = m_size % SIMD_WIDTH;

    const __m256 fx = _mm256_set1_ps(force_x);
    const __m256 fy = _mm256_set1_ps(force_y);

    for (size_t i = 0; i < SIMD_ITERS; ++i) {
        const size_t base = i * SIMD_WIDTH;

        __m256 inv_mass = _mm256_load_ps(m_inverted_mass.data() + base);
        __m256 ax       = _mm256_load_ps(m_acceleration_x.data() + base);
        __m256 ay       = _mm256_load_ps(m_acceleration_y.data() + base);

        ax = _mm256_fmadd_ps(fx, inv_mass, ax);
        ay = _mm256_fmadd_ps(fy, inv_mass, ay);

        _mm256_store_ps(m_acceleration_x.data() + base, ax);
        _mm256_store_ps(m_acceleration_y.data() + base, ay);
    }

    // tail
    for (size_t i = m_size - SIMD_REM; i < m_size; ++i) {
        m_acceleration_x[i] += force_x * m_inverted_mass[i];
        m_acceleration_y[i] += force_y * m_inverted_mass[i];
    }
}

void PointsSystem::integrate(float dt, float damping) noexcept {
    float dt2 = dt * dt;

    constexpr int SIMD_WIDTH = 8;
    const size_t SIMD_ITERS  = m_size / SIMD_WIDTH;
    const size_t SIMD_REM    = m_size % SIMD_WIDTH;

    const __m256 damp_m256 = _mm256_set1_ps(damping);
    const __m256 dt2_m256  = _mm256_set1_ps(dt2);

    for (size_t i = 0; i < SIMD_ITERS; ++i) {
        const size_t base = i * SIMD_WIDTH;

        __m256 posx = _mm256_load_ps(m_pos_x.data() + base);
        __m256 posy = _mm256_load_ps(m_pos_y.data() + base);

        __m256 prevPosx = _mm256_load_ps(m_prev_pos_x.data() + base);
        __m256 prevPosy = _mm256_load_ps(m_prev_pos_y.data() + base);

        _mm256_store_ps(m_prev_pos_x.data() + base, posx);
        _mm256_store_ps(m_prev_pos_y.data() + base, posy);

        __m256 acx = _mm256_load_ps(m_acceleration_x.data() + base);
        __m256 acy = _mm256_load_ps(m_acceleration_y.data() + base);

        prevPosx = _mm256_sub_ps(posx, prevPosx);
        prevPosy = _mm256_sub_ps(posy, prevPosy);

        posx = _mm256_fmadd_ps(damp_m256, prevPosx, posx);
        posy = _mm256_fmadd_ps(damp_m256, prevPosy, posy);

        posx = _mm256_fmadd_ps(acx, dt2_m256, posx);
        posy = _mm256_fmadd_ps(acy, dt2_m256, posy);

        _mm256_store_ps(m_pos_x.data() + base, posx);
        _mm256_store_ps(m_pos_y.data() + base, posy);

        _mm256_store_ps(m_acceleration_x.data() + base, _mm256_setzero_ps());
        _mm256_store_ps(m_acceleration_y.data() + base, _mm256_setzero_ps());
    }

    // tail
    const size_t scalar_base = SIMD_ITERS * SIMD_WIDTH;
    for (size_t i = m_size - SIMD_REM; i < m_size; ++i) {
        const float posx = m_pos_x[i];
        const float posy = m_pos_y[i];

        const float prevx = m_prev_pos_x[i];
        const float prevy = m_prev_pos_y[i];

        m_prev_pos_x[i] = posx;
        m_prev_pos_y[i] = posy;

        const float acx = m_acceleration_x[i];
        const float acy = m_acceleration_y[i];

        m_pos_x[i] = posx + (posx - prevx) * damping + acx * dt2;
        m_pos_y[i] = posy + (posy - prevy) * damping + acy * dt2;

        m_acceleration_x[i] = 0.0f;
        m_acceleration_y[i] = 0.0f;
    }
}

}  // namespace engine::objects