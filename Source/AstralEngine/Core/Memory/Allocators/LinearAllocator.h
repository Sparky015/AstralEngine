/**
* @file LinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include <cstddef>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the reset method to deallocate memory. It deallocates all memory being used.
     *          It's all or nothing. */
    template <size_t memoryBlockSize>
    class LinearAllocator
    {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(memoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        using AllocationHeader = uint8;


        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block
         * @throw std::bad_alloc When there is not enough memory to complete an allocation */
        void* allocate(size_t size, uint16 alignment)
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

            return alignedAddress;
        }


        /**@brief This does nothing. Use reset method to deallocate memory. */
        void deallocate(void* ptr, size_t sizeOfAllocatedBlock)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        size_t getUsedBlockSize()
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        LinearAllocator() noexcept = default;
        LinearAllocator(size_t) noexcept : LinearAllocator() {}
        ~LinearAllocator() { reset(); }

    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

        template<size_t S>
        friend bool operator==(const LinearAllocator<S>& a1, const LinearAllocator<S>& a2) noexcept;

        template<size_t S>
        friend bool operator==(const LinearAllocator<S>& a1, const LinearAllocator<S>& a2) noexcept;
    };


template <size_t memoryBlockSize>
bool operator==(const LinearAllocator<memoryBlockSize>& a1, const LinearAllocator<memoryBlockSize>& a2) noexcept
{
    return (a1.m_CurrentMarker == a2.m_CurrentMarker &&
        &a1.m_MemoryBlock == &a2.m_MemoryBlock &&
        a1.m_EndBlockAddress == a2.m_EndBlockAddress &&
        a1.m_StartBlockAddress == a2.m_StartBlockAddress);
}

template <size_t memoryBlockSize>
bool operator!=(const LinearAllocator<memoryBlockSize>& a1, const LinearAllocator<memoryBlockSize>& a2) noexcept
{
    return !(a1 == a2);
}

}