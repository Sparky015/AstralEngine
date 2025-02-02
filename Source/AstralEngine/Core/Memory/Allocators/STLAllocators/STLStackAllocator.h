/**
* @file STLStackAllocator.h
* @author Andrew Fagan
* @date 1/15/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include <memory>
#include <cstring>

#include "Core/Memory/Allocators/AllocatorUtils.h"
#include "Core/Memory/Allocators/StackAllocator.h"

namespace Core {

    /**@brief An STL compliant version of the StackAllocator. It is a stack-like allocator that allocates memory in a
     *        last in first out order. This means that the user can deallocate only the most recent unfreed memory allocation.
     * @thread_safety This class is NOT thread safe. */
    template <typename T>
    class STLStackAllocator
    {
    public:
        using value_type = T;
        using pointer = T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!

        explicit STLStackAllocator(size_type memoryBlock) :
                m_StackAllocator(std::make_shared<Core::StackAllocator>(memoryBlock))
        {}

        ~STLStackAllocator() = default;

        struct AllocationHeader
        {
            explicit AllocationHeader(const uint8 alignmentOffset) : alignmentOffset(alignmentOffset) {}
            uint8 alignmentOffset;
        };
        static_assert(sizeof(AllocationHeader) == 1, "The allocation header should only be one byte in size. "
                                                     "The TrackAllocation and Deallocate implementation assumes a size of 1 byte. ");

        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_StackAllocator->GetMarker(); }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(Marker marker)
        {
            m_StackAllocator->RollbackToMarker(marker);
        }

        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            return (pointer)m_StackAllocator->Allocate(allocatedBytes, alignof(T));
        }


        /**@brief Deallocates the memory block at the pointer
         * @warning You can only Deallocate the previous allocation. This allocator follows a last in first out approach */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            m_StackAllocator->Deallocate(ptr, numberOfElements * sizeof(T));
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            m_StackAllocator->Reset();
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t getUsedBlockSize() const
        {
            return m_StackAllocator->GetUsedBlockSize();
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t getCapacity() const
        {
            return m_StackAllocator->GetCapacity();
        }

        template <typename U>
        struct rebind
        {
            using other = STLStackAllocator<U>;
        };


        STLStackAllocator(const STLStackAllocator& other) :
            m_StackAllocator(other.m_StackAllocator)
        {}

        STLStackAllocator& operator=(const STLStackAllocator& other)
        {
            if (this != &other)
            {
                m_StackAllocator = other.m_StackAllocator;
            }
            return *this;
        }

        template <typename U>
        STLStackAllocator(const STLStackAllocator<U>& other) :
                m_StackAllocator(other.m_StackAllocator)
        {}

        template <typename U>
        STLStackAllocator& operator=(const STLStackAllocator<U>& other)
        {
            if (this != &other)
            {
                m_StackAllocator = other.m_StackAllocator;
            }
            return *this;
        }

        template <typename U>
        bool operator==(const STLStackAllocator<U>& other) noexcept
        {
            return (m_StackAllocator == other.m_StackAllocator);
        }

        template <typename U>
        bool operator!=(const STLStackAllocator<U>& other) noexcept
        {
            return !(*this == other);
        }

    private:
        template <typename U>
        friend class STLStackAllocator;

        std::shared_ptr<Core::StackAllocator> m_StackAllocator;
    };

}