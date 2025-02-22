/**
* @file NoTrackingSTLAllocator.h
* @author Andrew Fagan
* @date 1/7/2025
*/

#pragma once

#include <memory>
#include "Core/Memory/Allocators/NoTrackingMalloc.h"


namespace Core {

    /**@class NoTrackingSTLAllocator
     * @brief Allocator that bypasses allocation metrics tracking. Uses std::malloc and std::free.
     * @thread_safety This class is thread safe. */
    template <typename T>
    class NoTrackingSTLAllocator {
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
            void* ptr = ASTRAL_NO_TRACKING_MALLOC(n * sizeof(T));
      //      void* ptr = std::malloc(n * sizeof(T));
            if (!ptr) throw std::bad_alloc();
            return static_cast<pointer>(ptr);
        }


        /**@brief Deallocates memory using std::free */
        void deallocate(pointer ptr, size_type n)
        {
            ASTRAL_NO_TRACKING_FREE(ptr);
//            std::free(ptr);
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = NoTrackingSTLAllocator<U>;
        };


        NoTrackingSTLAllocator() noexcept = default;
        ~NoTrackingSTLAllocator() = default;

        template <typename U>
        NoTrackingSTLAllocator(const NoTrackingSTLAllocator<U>&) noexcept {};
        NoTrackingSTLAllocator(const NoTrackingSTLAllocator&) noexcept = default;
    };

    // Equality operators for allocators
    template <typename T, typename U>
    bool operator==(const NoTrackingSTLAllocator<T>&, const NoTrackingSTLAllocator<U>&) { return true; }

    template <typename T, typename U>
    bool operator!=(const NoTrackingSTLAllocator<T>&, const NoTrackingSTLAllocator<U>&) { return false; }

}