/**
* @file DoubleBufferedAllocator.h
* @author Andrew Fagan
* @date 1/21/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/NewDeleteOverrides.h"
#include "Debug/Macros/Asserts.h"
#include <cstring>
#include <memory>

#include "FrameAllocator.h"

namespace Core {

    /**@brief An allocator that can be used to allocate during a frame. Do not store or cache any pointers from this allocator.
     *        It will be deallocated after the frame has ended.
     * @thread_safety This class is NOT thread safe.
     * @note Copying is not allowed with this allocator. */
    class DoubleBufferedAllocator
    {
    public:

        explicit DoubleBufferedAllocator(size_t memoryBlockSize);
        ~DoubleBufferedAllocator() = default;


        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_ActiveBuffer->GetMarker(); }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker) { m_ActiveBuffer->RollbackToMarker(marker); }

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block or nullptr if the allocation failed. */
        void* Allocate(size_t size, uint16 alignment) { return m_ActiveBuffer->Allocate(size, alignment); }

        /** @brief Swaps the active buffer from the current buffer to the other buffer. */
        void SwapBuffers();

        /**@brief Resets ALL memory in the active buffer. Only the active buffer memory gets deallocated. */
        void ClearCurrentBuffer() { m_ActiveBuffer->Reset(); }

        /**@brief Gets the amount of memory currently allocated out by the active buffer.
         * @return The number of bytes currently allocated by the active buffer. */
        [[nodiscard]] size_t GetUsedBlockSize() const { return m_ActiveBuffer->GetUsedBlockSize(); }

        /**@brief Gets the memory capacity of the active buffer.
         * @return The max number of bytes the active buffer can allocate. */
        [[nodiscard]] size_t GetActiveBufferCapacity() const { return m_ActiveBuffer->GetCapacity(); }

        /**@brief Doubles the size of the active internal buffer of the allocator.
         * @return True if the resize operation succeeded and false if the operation failed.
         * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing. */
        [[nodiscard]] bool ResizeActiveBuffer();


        // Deleting copy constructor and operator because the copied data in use won't be able to be freed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they can't be freed.
        DoubleBufferedAllocator(const DoubleBufferedAllocator& other) = delete;
        DoubleBufferedAllocator& operator=(const DoubleBufferedAllocator& other) = delete;

        DoubleBufferedAllocator(DoubleBufferedAllocator&& other) noexcept;
        DoubleBufferedAllocator& operator=(DoubleBufferedAllocator&& other) noexcept;

        bool operator==(const DoubleBufferedAllocator& other) noexcept;
        bool operator!=(const DoubleBufferedAllocator& other) noexcept;


    private:
        FrameAllocator m_Buffers[2];
        FrameAllocator* m_ActiveBuffer;
    };

}
