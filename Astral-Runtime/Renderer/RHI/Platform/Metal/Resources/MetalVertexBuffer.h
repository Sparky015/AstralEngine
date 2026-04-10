/**
* @file MetalVertexBuffer.h
* @author Andrew Fagan
* @date 4/6/26
*/

#pragma once

#include "MetalBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
     * @brief A description of how to build a Metal vertex buffer
     */
    struct MetalVertexBufferDesc
    {
        MTL::Device* Device;
        void* VertexData;
        uint32 DataSize;
        VertexBufferLayout& BufferLayout;
        GPUMemoryType MemoryType;
    };

    /**
     * @brief A wrapper around a Metal buffer supporting vertex buffer layouts and extra convenience functions
     */
    class MetalVertexBuffer : public VertexBuffer
    {
    public:
        explicit MetalVertexBuffer(const MetalVertexBufferDesc& desc);
        ~MetalVertexBuffer() override;

        /**
         * @brief Gets the size of the vertex data initially written to this buffer
         * @return The size of the vertex data initially written to this buffer
         */
        uint32 GetSize() const override;

        /**
         * @brief Gets the layout of the vertex data in this buffer
         * @return The layout of the vertex data in this buffer
         */
        const VertexBufferLayout& GetBufferLayout() const override;

        /**
         * @brief Maps a pointer to the address of this buffer
         * @param cpuPtr The address of where the buffer address should be stored
         * @warning Use @ref CopyDataToBuffer to do write operations for accurate memory usage tracking of the buffer
         */
        void MapPointer(void** cpuPtr) override;

        /**
         * @brief Unmaps a pointer from this buffer
         * @note This does nothing on the Metal implementation
         */
        void UnmapPointer() override;

        /**
         * @brief Copies data to this buffer
         * @param data The address of the data to copy to the buffer
         * @param size The size of the data to copy to this buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a private buffer.
         */
        void CopyDataToBuffer(void* data, uint32 size) override;

        /**
        * @brief Uploads the given data to this buffer
        * @param data The address of the data to copy to the buffer
        * @param size The size of the data to upload to this buffer
        * @note This is only supported by private buffers. This does not support copying to a shared buffer.
        * @warning This forces a blocking wait while the GPU transfers the data.
        */
        void UploadToDeviceLocalBuffer(void* data, uint32 size) override;

        /**
         * @brief Changes the memory type used by this buffer to the requested type
         * @param memoryType The memory type to change the buffer to
         * @warning This forces a blocking wait while the GPU transfers the data between CPU to GPU or GPU to CPU memory
         */
        void ChangeMemoryType(GPUMemoryType memoryType) override;

        /**
         * @brief Gets the native handle of the buffer
         * @return The native handle of the buffer (MTL::Buffer*)
         */
        void* GetNativeHandle() override;

    private:

        /**
         * @brief Creates a vertex buffer from a vertex buffer description
         * @param desc The description of the vertex buffer to build
         */
        void CreateVertexBuffer(const MetalVertexBufferDesc& desc);

        VertexBufferLayout m_BufferLayout;
        MetalBuffer m_VertexBuffer;
        uint32 m_DataSize;
    };

}