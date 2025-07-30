/**
* @file GenericRegionAllocator.h
* @author Andrew Fagan
* @date 4/14/25
*/

#pragma once

#include "AllocationData.h"
#include "MemoryTracker.h"


namespace Astral {

    template <typename T>
    class GenericRegionAllocator
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Constructor for allocator.
         */
        explicit GenericRegionAllocator(MemoryRegion memoryRegion) : m_MemoryRegion(memoryRegion) {}

        /**
         * @brief Allocates memory with malloc
         * @throws bad_alloc if std::malloc returns nullptr
         */
        pointer allocate(size_type n, const void* hint = nullptr)
        {
            size_t allocationSize = n * sizeof(T);
            void* ptr = malloc(allocationSize);
            if (!ptr) throw std::bad_alloc();
            Astral::MemoryTracker::Get().AddAllocation(ptr, allocationSize, m_MemoryRegion, Astral::AllocatorType::NEW_OPERATOR);
            return static_cast<pointer>(ptr);
        }


        /**
         * @brief Deallocates memory using free
         */
        void deallocate(pointer ptr, size_type n)
        {
            Astral::MemoryTracker::Get().RemoveAllocation(ptr);
            free(ptr);
        }

        template <typename... Args>
        void construct(value_type* ptr, Args&&... args)
        {
            new (ptr) value_type(std::forward<Args>(args)...);
        }

        /**
         * @brief Destroy an object at a given pointer
         */
        void destroy(value_type* ptr)
        {
            if (ptr)
            {
                ptr->~value_type();
            }
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = GenericRegionAllocator<U>;
        };


        GenericRegionAllocator() noexcept = default;
        ~GenericRegionAllocator() = default;

        template <typename U>
        GenericRegionAllocator(const GenericRegionAllocator<U>& other) noexcept : m_MemoryRegion(other.m_MemoryRegion) {};
        GenericRegionAllocator(const GenericRegionAllocator&) noexcept = default;


        MemoryRegion m_MemoryRegion;
    };

    // Equality operators for allocators
    template <typename T, typename U>
    bool operator==(const GenericRegionAllocator<T>&, const GenericRegionAllocator<U>&) { return true; }

    template <typename T, typename U>
    bool operator!=(const GenericRegionAllocator<T>&, const GenericRegionAllocator<U>&) { return false; }

}