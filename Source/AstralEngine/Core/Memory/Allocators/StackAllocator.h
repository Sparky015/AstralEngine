//
// Created by Andrew Fagan on 1/15/25.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include <memory>

namespace Core {

    template <typename T, size_t memoryBlockSize>
    class StackAllocator
    {
    public:
        using value_type = T;
        using pointer = T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!

        struct AllocationHeader
        {
            explicit AllocationHeader(const uint8 alignmentOffset) : alignmentOffset(alignmentOffset) {}
            uint8 alignmentOffset;
        };
        static_assert(sizeof(AllocationHeader) == 1, "The allocation header should only be one byte in size. "
                                                     "The allocate and deallocate implementation assumes a size of 1 byte. ");

        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_CurrentMarker; }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker)
        {
            unsigned char* headerMarker = reinterpret_cast<unsigned char*>(marker) - 1;
            AllocationHeader* allocationHeader = reinterpret_cast<AllocationHeader*>(headerMarker);

            // Roll back the marker by the size of the allocation and the natural alignment offset
            m_CurrentMarker = marker - allocationHeader->alignmentOffset;
        }

        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            if (m_CurrentMarker + allocatedBytes > m_EndBlockAddress) { throw std::bad_alloc(); }

            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;

            // Aligns the address and will return nullptr if there is not enough space
            if (!std::align(alignof(T), allocatedBytes, alignedAddress, space)) { throw std::bad_alloc(); }

            if (static_cast<unsigned char*>(alignedAddress) == m_CurrentMarker)
            {
                // Address is already aligned. Push the address by the alignment of T to make room for allocation header.
                alignedAddress = static_cast<unsigned char*>(alignedAddress) + alignof(T);

                if (static_cast<unsigned char*>(alignedAddress) + allocatedBytes > m_EndBlockAddress) { throw std::bad_alloc(); }
            }

            // Add allocation header for alignment amount
            unsigned char* m_HeaderMarker = static_cast<unsigned char*>(alignedAddress) - 1;
            uint8 alignmentOffset = static_cast<unsigned char*>(alignedAddress) - m_CurrentMarker;
            AllocationHeader allocationHeader = AllocationHeader(alignmentOffset);
            *reinterpret_cast<AllocationHeader*>(m_HeaderMarker) = allocationHeader;

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + allocatedBytes;

            return static_cast<pointer>(alignedAddress);
        }


        /**@brief Deallocates the memory block at the pointer
         * @warning You can only deallocate the previous allocation. This allocator follows a last in first out approach */
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
            memset(m_CurrentMarker, 0, sizeOfAllocation + allocationHeader->alignmentOffset);
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            memset(m_MemoryBlock, 0, memoryBlockSize);
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
            using other = StackAllocator<U, memoryBlockSize>;
        };

        StackAllocator() noexcept = default;
        StackAllocator(size_type memoryBlock) noexcept : StackAllocator() {}
        ~StackAllocator() { reset(); }


    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

        template<typename T1, typename T2, size_t S>
        friend bool operator==(const StackAllocator<T1, S>& a1, const StackAllocator<T2, S>& a2) noexcept;

        template<typename T1, typename T2, size_t S>
        friend bool operator!=(const StackAllocator<T1, S>& a1, const StackAllocator<T2, S>& a2) noexcept;
    };


    template <typename T, typename U, size_t memoryBlockSize>
    bool operator==(const StackAllocator<T, memoryBlockSize>& a1, const StackAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return (a1.m_CurrentMarker == a2.m_CurrentMarker &&
            &a1.m_MemoryBlock == &a2.m_MemoryBlock &&
            a1.m_EndBlockAddress == a2.m_EndBlockAddress &&
            a1.m_StartBlockAddress == a2.m_StartBlockAddress);
    }

    template <typename T, typename U, size_t memoryBlockSize>
    bool operator!=(const StackAllocator<T, memoryBlockSize>& a1, const StackAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return !(a1 == a2);
    }

}