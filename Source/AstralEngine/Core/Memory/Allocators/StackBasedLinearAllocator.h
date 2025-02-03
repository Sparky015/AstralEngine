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
#include <cstring>


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
            AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, GetCapacity(), AllocatorUtils::FreedMemory);
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

            AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, (unsigned char*)alignedAddress - m_CurrentMarker, AllocatorUtils::AlignedOffsetFence);
            AllocatorUtils::SetMemoryRegionBoundary(alignedAddress, size, AllocatorUtils::AllocatedMemory);

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

            TRACK_ALLOCATION(size);
            return alignedAddress;
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset()
        {
            TRACK_DEALLOCATION(m_CurrentMarker - m_StartBlockAddress);
            AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetCapacity(), AllocatorUtils::FreedMemory);
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t GetUsedBlockSize() const
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t GetCapacity() const
        {
            return m_EndBlockAddress - m_StartBlockAddress;
        }

        StackBasedLinearAllocator(const StackBasedLinearAllocator& other) :
            m_MemoryBlock(),
            m_StartBlockAddress(&m_MemoryBlock[0]),
            m_EndBlockAddress(m_StartBlockAddress + MemoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress + other.GetUsedBlockSize())
        {
            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
            AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetUsedBlockSize(), AllocatorUtils::AllocatedMemory);
            AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), AllocatorUtils::FreedMemory);
        }

        StackBasedLinearAllocator& operator=(const StackBasedLinearAllocator& other)
        {
            if (this != &other)
            {
                std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
                m_StartBlockAddress = &m_MemoryBlock[0];
                m_EndBlockAddress = m_StartBlockAddress + MemoryBlockSize;
                m_CurrentMarker = m_StartBlockAddress + other.GetUsedBlockSize();
                AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetUsedBlockSize(), AllocatorUtils::AllocatedMemory);
                AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), AllocatorUtils::FreedMemory);
            }
            return *this;
        }

        StackBasedLinearAllocator(StackBasedLinearAllocator&& other) noexcept :
            m_MemoryBlock(),
            m_StartBlockAddress(&m_MemoryBlock[0]),
            m_EndBlockAddress(m_StartBlockAddress + MemoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress + other.GetUsedBlockSize())
        {
            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
            other.m_StartBlockAddress = nullptr;
            other.m_EndBlockAddress = nullptr;
            other.m_CurrentMarker = nullptr;
        }

        StackBasedLinearAllocator& operator=(StackBasedLinearAllocator&& other) noexcept
        {
            if (this != &other)
            {
                std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
                m_StartBlockAddress = &m_MemoryBlock[0];
                m_EndBlockAddress = m_StartBlockAddress + MemoryBlockSize;
                m_CurrentMarker = m_StartBlockAddress + other.GetUsedBlockSize();
                other.m_StartBlockAddress = nullptr;
                other.m_EndBlockAddress = nullptr;
                other.m_CurrentMarker = nullptr;
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
