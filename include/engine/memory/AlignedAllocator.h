#ifndef ALIGNED_ALLOCATOR_ENGINE_H
#define ALIGNED_ALLOCATOR_ENGINE_H

#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>

#include "engine/core/CompilerSettings.h"

namespace engine::memory {

template <typename T, size_t Align>
class AlignedAllocator {
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = AlignedAllocator<U, Align>;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;
    using is_always_equal                        = std::true_type;

    AlignedAllocator() noexcept  = default;
    ~AlignedAllocator() noexcept = default;

    template <typename U>
    AlignedAllocator(const AlignedAllocator<U, Align>&) noexcept {}

    T* allocate(size_t n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > std::numeric_limits<size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        void* p = _mm_malloc(n * sizeof(T), Align);
        if (!p)
            throw std::bad_alloc();
        return static_cast<T*>(p);
    }

    void deallocate(T* p, size_t) noexcept { _mm_free(p); }

    template <typename U, size_t A>
    bool operator==(const AlignedAllocator<U, A>&) const noexcept {
        return Align == A;
    }

    template <typename U, size_t A>
    bool operator!=(const AlignedAllocator<U, A>& other) const noexcept {
        return !(*this == other);
    }
};

}  // namespace engine::memory

#endif  // ALIGNED_ALLOCATOR_ENGINE_H