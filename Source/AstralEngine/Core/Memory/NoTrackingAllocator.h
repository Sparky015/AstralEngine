/**
* @file NoTrackingAllocator.h
* @author Andrew Fagan
* @date 1/7/2025
*/

#pragma once

namespace Core {

    /**@class NoTrackingAllocator
     * @brief Allocator that bypasses allocation metrics tracking. Uses std::malloc and std::free. */
    template <typename T>
    class NoTrackingAllocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;


        /**@brief Allocates memory with std::malloc
         * @throws bad_alloc if std::malloc returns nullptr */
        pointer allocate(size_type n, const void* hint = nullptr)
        {
            void* ptr = std::malloc(n * sizeof(T));
            if (!ptr) throw std::bad_alloc();
            return static_cast<pointer>(ptr);
        }


        /**@brief Deallocates memory using std::free */
        void deallocate(pointer ptr, size_type n)
        {
            std::free(ptr);
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = NoTrackingAllocator<U>;
        };


        NoTrackingAllocator() noexcept = default;
        ~NoTrackingAllocator() = default;

        template <typename U>
        NoTrackingAllocator(const NoTrackingAllocator<U>&) noexcept {};
        NoTrackingAllocator(const NoTrackingAllocator&) noexcept = default;

    };

    // Equality operators for allocators
    template <typename T, typename U>
    bool operator==(const NoTrackingAllocator<T>&, const NoTrackingAllocator<U>&) { return true; }

    template <typename T, typename U>
    bool operator!=(const NoTrackingAllocator<T>&, const NoTrackingAllocator<U>&) { return false; }

}