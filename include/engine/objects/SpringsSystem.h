#ifndef SPRINGS_SYSTEM_H
#define SPRINGS_SYSTEM_H

#include <cstdint>

#include "engine/memory/AlignedAllocator.h"
#include "engine/objects/PointsSystem.h"

namespace engine::objects {

class SpringsSystem {
private:
    std::vector<uint32_t, memory::AlignedAllocator<uint32_t, 32>> m_start_points;
    std::vector<uint32_t, memory::AlignedAllocator<uint32_t, 32>> m_end_points;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_stiffness;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_rest_length;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_min_length;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_max_length;
    std::vector<float, memory::AlignedAllocator<float, 32>> m_damping;

    std::vector<uint32_t> m_free_indices;
    std::vector<uint32_t, memory::AlignedAllocator<uint32_t, 32>> m_is_active;

    PointsSystem* m_points_system;

    size_t m_size;
    size_t m_capacity;

public:
    explicit SpringsSystem(PointsSystem* points_system);

    uint32_t addSpring(uint32_t start_point, uint32_t end_point, float stiffness, float damping,
                       float rest_length = -1.0f, float min_length = -1.0f, float max_length = -1.0f);
    void removeSpring(uint32_t idx) noexcept;
    void clear() noexcept;

    void reserve(size_t new_capacity) noexcept;
    [[nodiscard]] size_t size() const noexcept { return m_size; }
    [[nodiscard]] size_t capacity() const noexcept { return m_capacity; }

    void applySpringsForce(float dt) noexcept;

    [[nodiscard]] const uint32_t* startIndices() const noexcept { return m_start_points.data(); }
    uint32_t* startIndices() noexcept { return m_start_points.data(); }
    [[nodiscard]] const uint32_t* endIndices() const noexcept { return m_end_points.data(); }
    uint32_t* endIndices() noexcept { return m_end_points.data(); }
    [[nodiscard]] const float* stiffness() const noexcept { return m_stiffness.data(); }
    float* stiffness() noexcept { return m_stiffness.data(); }
    [[nodiscard]] const float* restLenghts() const noexcept { return m_rest_length.data(); }
    float* restLenghts() noexcept { return m_rest_length.data(); }
    [[nodiscard]] const float* minLenghts() const noexcept { return m_min_length.data(); }
    float* minLenghts() noexcept { return m_min_length.data(); }
    [[nodiscard]] const float* maxLenghts() const noexcept { return m_max_length.data(); }
    float* maxLenghts() noexcept { return m_max_length.data(); }
    [[nodiscard]] const float* damping() const noexcept { return m_damping.data(); }
    float* damping() noexcept { return m_damping.data(); }
    [[nodiscard]] const uint32_t* isActive() const noexcept { return m_is_active.data(); }
    uint32_t* isActive() noexcept { return m_is_active.data(); }
};

}  // namespace engine::objects

#endif  // SPRINGS_SYSTEM_H