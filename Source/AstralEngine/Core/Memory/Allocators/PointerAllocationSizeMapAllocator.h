/**
* @file PointerAllocationSizeMapAllocator.h
* @author Andrew Fagan
* @date 1/7/2025
*/

#pragma once

namespace Core {

    template <typename T>
    class PointerAllocationSizeMapAllocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;


        pointer allocate(size_type n, const void* hint = nullptr)
        {
            void* ptr = std::malloc(n * sizeof(T));
            if (!ptr) throw std::bad_alloc();
            return static_cast<pointer>(ptr);
        }

        void deallocate(pointer ptr, size_type n)
        {
            std::free(ptr);
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = PointerAllocationSizeMapAllocator<U>;
        };


        PointerAllocationSizeMapAllocator() noexcept = default;
        ~PointerAllocationSizeMapAllocator() = default;

        template <typename U>
        PointerAllocationSizeMapAllocator(const PointerAllocationSizeMapAllocator<U>&) noexcept {};
        PointerAllocationSizeMapAllocator(const PointerAllocationSizeMapAllocator&) noexcept = default;

    };

    // Equality operators for allocators
    template <typename T, typename U>
    bool operator==(const PointerAllocationSizeMapAllocator<T>&, const PointerAllocationSizeMapAllocator<U>&) { return true; }

    template <typename T, typename U>
    bool operator!=(const PointerAllocationSizeMapAllocator<T>&, const PointerAllocationSizeMapAllocator<U>&) { return false; }

}