/**
* @file Buffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    /**
     * @brief Supports buffer operations to store memory for GPU access
     */
    class Buffer
    {
    public:
        virtual ~Buffer() = default;

        /**
         * @brief Gets the amount of memory allocated to the buffer
         * @return The amount of memory used in the buffer
         */
        virtual uint32 GetAllocatedSize() = 0;

        /**
         * @brief Gets the amount of memory written to in the buffer
         * @return The amount of memory used in the buffer
         * @warning This is possible to be inaccurate if writing to a MapPointer was done
         */
        virtual uint32 GetUsedSize() = 0;

        /**
         * @brief Reallocates the length of a buffer by creating a new buffer and copying data over
         * @param newSize The length to make the new buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer. Reallocation is
         * skipped if the new buffer size is less than or equal to the current buffer size.
         */
        virtual void ReallocateMemory(uint32 newSize) = 0;

        /**
         * @brief Maps a pointer to the address of this buffer
         * @param cpuPtr The address of where the buffer address should be stored
         * @warning Use @ref CopyDataToBuffer to do write operations for accurate memory usage tracking of the buffer
         */
        virtual void MapPointer(void** cpuPtr) = 0;

        /**
         * @brief Unmaps a pointer from this buffer
         */
        virtual void UnmapPointer() = 0;

        /**
         * @brief Copies data to this buffer
         * @param data The address of the data to copy to the buffer
         * @param size The size of the data to copy to this buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer
         */
        virtual void CopyDataToBuffer(void* data, uint32 size) = 0;

        /**
         * @brief Gets the native handle of the buffer
         * @return The native handle of the buffer
         */
        virtual void* GetNativeHandle() = 0;

    };

    using BufferHandle = GraphicsRef<Buffer>;

}
