//
// Created by Andrew Fagan on 1/15/25.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include <memory>

#include "Debug/Macros/Asserts.h"

namespace Core {

    template <size_t memoryBlockSize>
    class StackAllocator
    {
    public:

        using AllocationHeader = uint8;

        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_CurrentMarker; }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker)
        {
            ASSERT(marker >= m_StartBlockAddress && marker <= m_EndBlockAddress, "Passed marker does not fall within this allocators memory block.")
            ASSERT(marker <= m_CurrentMarker, "Can not rollback to marker that is already past the top of the stack.")
            TRACK_DEALLOCATION(m_CurrentMarker - marker);
            m_CurrentMarker = marker;
        }

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block
         * @throw std::bad_alloc When there is not enough memory to complete an allocation */
        void* Allocate(size_t size, uint16 alignment)
        {
            if (m_CurrentMarker + size > m_EndBlockAddress) { throw std::bad_alloc(); }

            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;

            // Aligns the address and will return nullptr if there is not enough space
            if (!std::align(alignment, size, alignedAddress, space)) { throw std::bad_alloc(); }

            if (static_cast<unsigned char*>(alignedAddress) == m_CurrentMarker)
            {
                // Address is already aligned. Push the address by the alignment of T to make room for allocation header.
                alignedAddress = static_cast<unsigned char*>(alignedAddress) + alignment;
                if (static_cast<unsigned char*>(alignedAddress) + size > m_EndBlockAddress) { throw std::bad_alloc(); }
            }

            // Add allocation header for alignment amount
            unsigned char* m_HeaderMarker = static_cast<unsigned char*>(alignedAddress) - 1;
            const uint8 alignmentOffset = static_cast<unsigned char*>(alignedAddress) - m_CurrentMarker;
            *(m_HeaderMarker) = alignmentOffset;

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

            TRACK_ALLOCATION(size);
            return alignedAddress;
        }


        /**@brief Deallocates the memory block at the pointer
         * @warning You can only Deallocate the previous allocation. This allocator follows a last in first out approach */
        void Deallocate(void* ptr, size_t sizeOfAllocatedBlock)
        {
            // Checking if this pointer is the last allocated pointer
            if (m_CurrentMarker - sizeOfAllocatedBlock != static_cast<unsigned char*>(ptr)) { throw std::runtime_error("Deallocations must follow a last in first out order!"); }

            // Get the natural alignment offset size from the allocation header
            AllocationHeader alignmentOffset = 0;
            if (static_cast<unsigned char*>(ptr) != m_StartBlockAddress)
            {
                unsigned char* headerMarker = static_cast<unsigned char*>(ptr) - 1;
                alignmentOffset = *headerMarker;
            }

            // Roll back the marker by the size of the allocation and the natural alignment offset
            m_CurrentMarker -= sizeOfAllocatedBlock + alignmentOffset;

            TRACK_DEALLOCATION(sizeOfAllocatedBlock);
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset()
        {
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t GetUsedBlockSize() const
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }


        StackAllocator() noexcept = default;
        StackAllocator(size_t memoryBlock) noexcept : StackAllocator() {}
        ~StackAllocator() { Reset(); }


    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

        template<size_t S>
        friend bool operator==(const StackAllocator<S>& a1, const StackAllocator<S>& a2) noexcept;

        template<size_t S>
        friend bool operator!=(const StackAllocator<S>& a1, const StackAllocator<S>& a2) noexcept;
    };


    template <size_t memoryBlockSize>
    bool operator==(const StackAllocator<memoryBlockSize>& a1, const StackAllocator<memoryBlockSize>& a2) noexcept
    {
        return (a1.m_CurrentMarker == a2.m_CurrentMarker &&
            &a1.m_MemoryBlock == &a2.m_MemoryBlock &&
            a1.m_EndBlockAddress == a2.m_EndBlockAddress &&
            a1.m_StartBlockAddress == a2.m_StartBlockAddress);
    }

    template <size_t memoryBlockSize>
    bool operator!=(const StackAllocator<memoryBlockSize>& a1, const StackAllocator<memoryBlockSize>& a2) noexcept
    {
        return !(a1 == a2);
    }

}
