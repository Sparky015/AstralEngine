/**
* @file FrameAllocator.h
* @author Andrew Fagan
* @date 1/20/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include "Debug/Macros/Asserts.h"
#include <memory>

namespace Core {

    /**@brief An allocator that can be used to allocate during a frame. Do not store or cache any pointers from this allocator.
     *        It will be deallocated after the frame has ended. */
    class FrameAllocator
    {
    public:

        explicit FrameAllocator(size_t memoryBlockSize) :
            m_MemoryBlockSize(memoryBlockSize),
            m_StartBlockAddress(new unsigned char[memoryBlockSize])
        {
            ASSERT(memoryBlockSize > 0, "The memory block size must be greater than 0");
        }

        ~FrameAllocator()
        {
            delete[] m_StartBlockAddress;
        }

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

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

            TRACK_ALLOCATION(size);
            return alignedAddress;
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

        FrameAllocator(const FrameAllocator&) = delete;
        FrameAllocator& operator=(const FrameAllocator&) = delete;

    private:
        size_t m_MemoryBlockSize;
        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + m_MemoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

    };

}