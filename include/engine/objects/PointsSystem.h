#ifndef POINT_ENGINE_H
#define POINT_ENGINE_H

#include <cstdint>
#include <vector>

#include "engine/core/CompilerSettings.h"
#include "engine/memory/AlignedAllocator.h"

namespace engine::objects {

class PointsSystem {
private:
    std::vector<float, memory::AlignedAllocator<float, 32>> m_pos_x;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_pos_y;

    std::vector<float, memory::AlignedAllocator<float, 32>> m_prev_pos_x;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_prev_pos_y;

    std::vector<float, memory::AlignedAllocator<float, 32>> m_acceleration_x;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_acceleration_y;

    std::vector<float, memory::AlignedAllocator<float, 32>> m_mass;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_inverted_mass;

    std::vector<uint32_t, memory::AlignedAllocator<uint32_t, 32>> m_is_active;
    std::vector<uint32_t> m_free_indices;

    size_t m_size;
    size_t m_capacity;

public:
    PointsSystem();
    ~PointsSystem() = default;

    uint32_t addPoint(float x, float y, float mass);
    void removePoint(size_t idx) noexcept;
    void clear() noexcept;

    void reserve(size_t new_capacity) noexcept;
    [[nodiscard]] size_t size() const noexcept { return m_size; }
    [[nodiscard]] size_t capacity() const noexcept { return m_capacity; }

    void applyForce(size_t idx, float force_x, float force_y) noexcept;
    void applyGlobalForce(float force_x, float force_y) noexcept;
    void integrate(float dt, float damping = 0.95f) noexcept;

    [[nodiscard]] const float* positionsX() const noexcept { return m_pos_x.data(); }
    float* positionsX() noexcept { return m_pos_x.data(); }
    [[nodiscard]] const float* positionsY() const noexcept { return m_pos_y.data(); }
    float* positionsY() noexcept { return m_pos_y.data(); }
    [[nodiscard]] const float* prevPositionsX() const noexcept { return m_prev_pos_x.data(); }
    float* prevPositionsX() noexcept { return m_prev_pos_x.data(); }
    [[nodiscard]] const float* prevPositionsY() const noexcept { return m_prev_pos_y.data(); }
    float* prevPositionsY() noexcept { return m_prev_pos_y.data(); }
    [[nodiscard]] const float* accelerationsX() const noexcept { return m_acceleration_x.data(); }
    float* accelerationsX() noexcept { return m_acceleration_x.data(); }
    [[nodiscard]] const float* accelerationsY() const noexcept { return m_acceleration_y.data(); }
    float* accelerationsY() noexcept { return m_acceleration_y.data(); }
    [[nodiscard]] const float* masses() const noexcept { return m_mass.data(); }
    float* masses() noexcept { return m_mass.data(); }
    [[nodiscard]] const float* invertedMasses() const noexcept { return m_inverted_mass.data(); }
    float* invertedMasses() noexcept { return m_inverted_mass.data(); }
    [[nodiscard]] const uint32_t* isActive() const noexcept { return m_is_active.data(); }
    uint32_t* isActive() noexcept { return m_is_active.data(); }
};

}  // namespace engine::objects

#endif  // POINT_ENGINE_H