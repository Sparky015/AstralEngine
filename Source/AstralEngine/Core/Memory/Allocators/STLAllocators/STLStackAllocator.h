/**
* @file STLStackAllocator.h
* @author Andrew Fagan
* @date 1/15/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include <memory>

#include "Core/Memory/Allocators/AllocatorUtils.h"

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
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlock)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlock),
            m_CurrentMarker(m_StartBlockAddress)
        {}

        ~STLStackAllocator()
        {
            AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
        }

        struct AllocationHeader
        {
            explicit AllocationHeader(const uint8 alignmentOffset) : alignmentOffset(alignmentOffset) {}
            uint8 alignmentOffset;
        };
        static_assert(sizeof(AllocationHeader) == 1, "The allocation header should only be one byte in size. "
                                                     "The TrackAllocation and Deallocate implementation assumes a size of 1 byte. ");

        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_CurrentMarker; }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker)
        {
            AllocationHeader* allocationHeader = reinterpret_cast<AllocationHeader*>(marker - 1);

            // Roll back the marker by the size the natural alignment offset
            m_CurrentMarker = marker - allocationHeader->alignmentOffset;
        }

        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, allocatedBytes, m_EndBlockAddress))
            {
                throw std::bad_alloc();
            }

            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;

            // Aligns the address and will return nullptr if there is not enough space
            if (!std::align(alignof(T), allocatedBytes, alignedAddress, space)) { throw std::bad_alloc(); }

            if (static_cast<unsigned char*>(alignedAddress) == m_CurrentMarker)
            {
                // Address is already aligned. Push the address by the alignment of T to make room for allocation header.
                alignedAddress = static_cast<unsigned char*>(alignedAddress) + alignof(T);

                if (AllocatorUtils::DoesCauseOverflow(alignedAddress, allocatedBytes, m_EndBlockAddress))
                {
                    throw std::bad_alloc();
                }
            }

            // Add allocation header for alignment amount
            unsigned char* m_HeaderMarker = static_cast<unsigned char*>(alignedAddress) - 1;
            uint8 alignmentOffset = static_cast<unsigned char*>(alignedAddress) - m_CurrentMarker;
            AllocationHeader allocationHeader = AllocationHeader(alignmentOffset);
            *reinterpret_cast<AllocationHeader*>(m_HeaderMarker) = allocationHeader;

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + allocatedBytes;

            TRACK_ALLOCATION(allocatedBytes);

            return static_cast<pointer>(alignedAddress);
        }


        /**@brief Deallocates the memory block at the pointer
         * @warning You can only Deallocate the previous allocation. This allocator follows a last in first out approach */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Checking if this pointer is the last allocated pointer
            size_t sizeOfAllocation = sizeof(T) * numberOfElements;
            if (m_CurrentMarker - sizeOfAllocation != reinterpret_cast<unsigned char*>(ptr)) { throw std::runtime_error("Deallocations must follow a last in first out order!"); }

            // Get the natural alignment offset size from the allocation header
            unsigned char* headerMarker = reinterpret_cast<unsigned char*>(ptr) - 1;
            AllocationHeader* allocationHeader = reinterpret_cast<AllocationHeader*>(headerMarker);

            // Roll back the marker by the size of the allocation and the natural alignment offset
            m_CurrentMarker -= sizeOfAllocation + allocationHeader->alignmentOffset;

            TRACK_DEALLOCATION(sizeOfAllocation);
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t getUsedBlockSize() const
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        template <typename U>
        struct rebind
        {
            using other = STLStackAllocator<U>;
        };


        template <typename U>
        bool operator==(const STLStackAllocator<U>& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        template <typename U>
        bool operator!=(const STLStackAllocator<U>& other) noexcept
        {
            return !(*this == other);
        }

    private:

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;
    };

}