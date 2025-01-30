/**
* @file StackBasedLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include "Debug/Macros/Asserts.h"
#include <cstddef>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    template<size_t MemoryBlockSize>
    class StackBasedLinearAllocator {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(MemoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        StackBasedLinearAllocator() :
            m_MemoryBlock(),
            m_StartBlockAddress(&m_MemoryBlock[0]),
            m_EndBlockAddress(m_StartBlockAddress + MemoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
        {}

        ~StackBasedLinearAllocator()
        {
            Reset();
        }


        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block
         * @throw std::bad_alloc When there is not enough memory to complete an allocation */
        void* Allocate(size_t size, uint16 alignment)
        {
            ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
            if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { throw std::bad_alloc(); }

            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;

            // Aligns the address and will return nullptr if there is not enough space
            if (!std::align(alignment, size, alignedAddress, space)) { throw std::bad_alloc(); }

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

            TRACK_ALLOCATION(size);
            return alignedAddress;
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset()
        {
            TRACK_DEALLOCATION(m_CurrentMarker - m_StartBlockAddress);
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        size_t GetUsedBlockSize()
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }


        StackBasedLinearAllocator(const StackBasedLinearAllocator& other)
        {

        }

        StackBasedLinearAllocator& operator=(const StackBasedLinearAllocator& other)
        {
            if (this != &other)
            {

            }
            return *this;
        }

        StackBasedLinearAllocator(StackBasedLinearAllocator&& other) noexcept
        {

        }

        StackBasedLinearAllocator& operator=(StackBasedLinearAllocator&& other) noexcept
        {
            if (this != &other)
            {

            }
            return *this;
        }


        bool operator==(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        bool operator!=(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept
        {
            return !(*this == other);
        }

    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[MemoryBlockSize];
        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;


    };

}
