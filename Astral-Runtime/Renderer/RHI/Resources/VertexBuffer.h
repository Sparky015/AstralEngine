//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/MemoryTypes.h"
#include "VertexBufferLayout.h"

namespace Astral {

    /**
     * @brief Supports buffer operations to store vertex data for GPU access and tracks vertex data layout
     */
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        /**
         * @brief Gets the size of the vertex data initially written to this buffer
         * @return The size of the vertex data initially written to this buffer
         */
        virtual uint32 GetSize() const = 0;

        /**
         * @brief Gets the layout of the vertex data in this buffer
         * @return The layout of the vertex data in this buffer
         */
        virtual const VertexBufferLayout& GetBufferLayout() const = 0;

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
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer.
         */
        virtual void CopyDataToBuffer(void* data, uint32 size) = 0;

        /**
        * @brief Uploads the given data to this buffer
        * @param data The address of the data to copy to the buffer
        * @param size The size of the data to upload to this buffer
        * @note This is only supported by device local buffers. This does not support copying to a host visible buffer.
        * @warning This forces a blocking wait while the GPU transfers the data.
        */
        virtual void UploadToDeviceLocalBuffer(void* data, uint32 size) = 0;

        /**
         * @brief Changes the memory type used by this buffer to the requested type
         * @param memoryType The memory type to change the buffer to
         * @warning This forces a blocking wait while the GPU transfers the data between CPU to GPU or GPU to CPU memory
         */
        virtual void ChangeMemoryType(GPUMemoryType memoryType) = 0;

        /**
         * @brief Gets the native handle of the buffer
         * @return The native handle of the buffer
         */
        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<VertexBuffer> CreateVertexBuffer(float* vertices, unsigned int size, VertexBufferLayout& bufferLayout);
    };

    using VertexBufferHandle = GraphicsRef<VertexBuffer>;

} // Renderer
