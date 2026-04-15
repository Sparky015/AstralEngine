/**
* @file MetalIndexBuffer.h
* @author Andrew Fagan
* @date 4/10/26
*/

#pragma once

#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "MetalBuffer.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
     * @brief A description of how to build a Metal index buffer
     */
    struct MetalIndexBufferDesc
    {
        MTL::Device* Device;
        uint32* IndexData;
        uint32 DataSize;
        GPUMemoryType MemoryType;
    };

    /**
     * @brief A wrapper around a Metal buffer supporting extra convenience functions for an index buffer
     */
    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        explicit MetalIndexBuffer(const MetalIndexBufferDesc& desc);
        ~MetalIndexBuffer() override;

        /**
         * @brief Gets the number of indices in the buffer
         * @return The number of indices in the buffer
         */
        uint32 GetCount() const override;

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

        MetalIndexBuffer(const MetalIndexBuffer&) = delete;
        MetalIndexBuffer& operator=(const MetalIndexBuffer& desc) = delete;
        MetalIndexBuffer(MetalIndexBuffer&& other) noexcept;
        MetalIndexBuffer& operator=(MetalIndexBuffer&& other) noexcept;

    private:

        /**
         * @brief Creates an index buffer from an index buffer description
         * @param desc The description of the index buffer to build
         */
        void CreateIndexBuffer(const MetalIndexBufferDesc& desc);

        MetalBuffer m_IndexBuffer;
        uint32 m_DataSize;
    };

}