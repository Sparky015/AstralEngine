/**
* @file LinearAllocator.h
* @author Andrew Fagan
* @date 1/29/25
*/

#pragma once

#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include "Debug/Macros/Asserts.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */

    class LinearAllocator {
    public:

        explicit LinearAllocator(size_t memoryBlockSize) :
                m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
                m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
                m_CurrentMarker(m_StartBlockAddress)
        {}

        ~LinearAllocator()
        {
            AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
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

        /**@brief Doubles the size of the internal buffer of the allocator.
         * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing. */
        void ResizeBuffer()
        {
            if (GetUsedBlockSize() != 0) { return; }
            ResizeInternalMemoryBlock();
        }

        LinearAllocator(const LinearAllocator& other) :
                m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.GetCapacity())),
                m_EndBlockAddress(m_StartBlockAddress + other.GetCapacity()),
                m_CurrentMarker(m_StartBlockAddress + other.GetUsedBlockSize())
        {
            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.GetCapacity());
            AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetUsedBlockSize(), AllocatorUtils::AllocatedMemory);
            AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), AllocatorUtils::FreedMemory);
        }

        LinearAllocator& operator=(const LinearAllocator& other)
        {
            if (this != &other)
            {
                m_StartBlockAddress = (unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.GetCapacity());
                std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.GetCapacity());
                m_EndBlockAddress = m_StartBlockAddress + other.GetCapacity();
                m_CurrentMarker = m_StartBlockAddress + other.GetUsedBlockSize();
                AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetUsedBlockSize(), AllocatorUtils::AllocatedMemory);
                AllocatorUtils::SetMemoryRegionBoundary(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), AllocatorUtils::FreedMemory);
            }
            return *this;
        }

        LinearAllocator(LinearAllocator&& other) noexcept :
        m_StartBlockAddress(other.m_StartBlockAddress),
        m_EndBlockAddress(other.m_EndBlockAddress),
        m_CurrentMarker(other.m_CurrentMarker)
        {
            other.m_StartBlockAddress = nullptr;
            other.m_EndBlockAddress = nullptr;
            other.m_CurrentMarker = nullptr;
        }

        LinearAllocator& operator=(LinearAllocator&& other) noexcept
        {
            if (this != &other)
            {
                m_StartBlockAddress = other.m_StartBlockAddress;
                m_EndBlockAddress = other.m_EndBlockAddress;
                m_CurrentMarker = other.m_CurrentMarker;
                other.m_StartBlockAddress = nullptr;
                other.m_EndBlockAddress = nullptr;
                other.m_CurrentMarker = nullptr;
            }
            return *this;
        }


        bool operator==(const LinearAllocator& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        bool operator!=(const LinearAllocator& other) noexcept
        {
            return !(*this == other);
        }

    private:

        /** @brief Attempts to resize the internal buffer of the allocator. This should only be used when the
         *         allocator is empty.
         *  @throw std::bad_alloc Throws when resize allocation failed to allocate a new block. */
        inline void ResizeInternalMemoryBlock()
        {
            [[unlikely]] if (GetUsedBlockSize() != 0) { return; }

            void* newMemoryBlock = nullptr;
            size_t newMemoryBufferSize = 0;

            AllocatorUtils::ResizeMemoryBlock(m_StartBlockAddress, GetCapacity(), newMemoryBlock, newMemoryBufferSize);
            if (!newMemoryBlock) { throw std::bad_alloc(); }

            m_StartBlockAddress = (unsigned char*)newMemoryBlock;
            m_CurrentMarker = m_StartBlockAddress;
            m_EndBlockAddress = m_StartBlockAddress + newMemoryBufferSize;

            AllocatorUtils::SetMemoryRegionBoundary(m_StartBlockAddress, GetCapacity(), AllocatorUtils::FreedMemory);
        }

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}
