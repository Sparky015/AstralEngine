/**
* @file DoubleBufferedAllocator.h
* @author Andrew Fagan
* @date 1/21/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include "Debug/Macros/Asserts.h"
#include <cstring>
#include <memory>

#include "FrameAllocator.h"

namespace Core {

    /**@brief An allocator that can be used to allocate during a frame. Do not store or cache any pointers from this allocator.
     *        It will be deallocated after the frame has ended.
     * @thread_safety This class is NOT thread safe. */
    class DoubleBufferedAllocator
    {
    public:
        explicit DoubleBufferedAllocator(size_t memoryBlockSize) :
              m_Buffers{FrameAllocator(memoryBlockSize), FrameAllocator(memoryBlockSize)},
              m_ActiveBuffer(&m_Buffers[0])
        {
            ASSERT(memoryBlockSize > 0, "The memory block size must be greater than 0");
        }

        ~DoubleBufferedAllocator() = default;

        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_ActiveBuffer->GetMarker(); }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker)
        {
            m_ActiveBuffer->RollbackToMarker(marker);
        }

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block
         * @throw std::bad_alloc When there is not enough memory to complete an allocation */
        void* Allocate(size_t size, uint16 alignment)
        {
            return m_ActiveBuffer->Allocate(size, alignment);
        }

        void SwapBuffers()
        {
            if (m_ActiveBuffer == &m_Buffers[0])
            {
                m_ActiveBuffer = &m_Buffers[1];
            }
            else
            {
                m_ActiveBuffer = &m_Buffers[0];
            }
        }

        /**@brief Resets ALL memory in the active buffer. Only the active buffer memory gets deallocated. */
        void ClearCurrentBuffer()
        {
            m_ActiveBuffer->Reset();
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t GetUsedBlockSize() const
        {
            return m_ActiveBuffer->GetUsedBlockSize();
        }

        [[nodiscard]] size_t GetActiveBufferCapacity() const
        {
            return m_ActiveBuffer->GetCapacity();
        }

        /**@brief Doubles the size of the active internal buffer of the allocator.
          * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing. */
        void ResizeActiveBuffer()
        {
            if (GetUsedBlockSize() != 0) { return; }
            m_ActiveBuffer->ResizeBuffer();
        }

        DoubleBufferedAllocator(const DoubleBufferedAllocator& other) :
            m_Buffers{other.m_Buffers[0], other.m_Buffers[1]},
            m_ActiveBuffer(other.m_ActiveBuffer)
        {}

        DoubleBufferedAllocator& operator=(const DoubleBufferedAllocator& other)
        {
            if (this != &other)
            {
                m_Buffers[0] = other.m_Buffers[0];
                m_Buffers[1] = other.m_Buffers[1];
                m_ActiveBuffer = other.m_ActiveBuffer;
            }
            return *this;
        }

        DoubleBufferedAllocator(DoubleBufferedAllocator&& other) noexcept :
            m_Buffers{std::move(other.m_Buffers[0]), std::move(other.m_Buffers[1])},
            m_ActiveBuffer(other.m_ActiveBuffer == &other.m_Buffers[0] ? &m_Buffers[0] : &m_Buffers[1])
        {
            other.m_ActiveBuffer = nullptr;
        }

        DoubleBufferedAllocator& operator=(DoubleBufferedAllocator&& other) noexcept
        {
            if (this != &other)
            {
                m_ActiveBuffer = other.m_ActiveBuffer == &other.m_Buffers[0] ? &m_Buffers[0] : &m_Buffers[1];
                m_Buffers[0] = std::move(other.m_Buffers[0]);
                m_Buffers[1] = std::move(other.m_Buffers[1]);
                other.m_ActiveBuffer = nullptr;
            }
            return *this;
        }

        bool operator==(const DoubleBufferedAllocator& other) noexcept
        {
            return (&m_Buffers[0] == &other.m_Buffers[0] &&
                    m_ActiveBuffer == other.m_ActiveBuffer);
        }

        bool operator!=(const DoubleBufferedAllocator& other) noexcept
        {
            return !(*this == other);
        }

    private:
        FrameAllocator m_Buffers[2];
        FrameAllocator* m_ActiveBuffer;
    };

}
