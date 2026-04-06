/**
* @file MetalBuffer.h
* @author Andrew Fagan
* @date 4/4/26
*/

#pragma once

#include "Metal/MTLBuffer.hpp"
#include "Renderer/RHI/Common/MemoryTypes.h"
#include "Renderer/RHI/Resources/Buffer.h"
#include "Renderer/RHI/Resources/Device.h"

namespace Astral {

    /**
     * @brief A description of how to build a Metal buffer
     */
    struct MetalBufferDesc
    {
        MTL::Device* Device;
        uint32 Size;
        GPUMemoryType MemoryType;
    };

    /**
     * @brief A wrapper around a Metal buffer supporting extra convenience functions
     */
    class MetalBuffer : public Buffer
    {
    public:
        MetalBuffer(const MetalBufferDesc& metalBufferDesc);
        ~MetalBuffer() override;

        /**
         * @brief Gets the amount of memory allocated to the buffer
         * @return The amount of memory used in the buffer
         */
        uint32 GetAllocatedSize() override;

        /**
         * @brief Gets the amount of memory written to in the buffer
         * @return The amount of memory used in the buffer
         */
        uint32 GetUsedSize() override;

        /**
         * @brief Reallocates the length of a buffer by creating a new buffer and copying data over
         * @param newSize The length to make the new buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a private buffer. Reallocation is
         * skipped if the new buffer size is less than or equal to the current buffer size.
         */
        void ReallocateMemory(uint32 newSize) override;

        /**
         * @brief Maps a pointer to the address of this buffer
         * @param cpuPtr The address of where the buffer address should be stored
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
         * @note This is only supported by host visible buffers. This does not support uploading to a private buffer
         */
        void CopyDataToBuffer(void* data, uint32 size) override;

        /**
         * @brief Gets the native handle of the buffer
         * @return The native handle of the buffer (MTL::Buffer*)
        */
        void* GetNativeHandle() override;

        /**
         * @brief Copies data from a staging buffer to this buffer using a blit command
         * @param device The graphics device
         * @param stagingBuffer The origin buffer to copy from
         * @param size The size of the data that should be copied to this buffer
         * @note This forces a blocking wait while the GPU transfers the data
         */
        void CopyFromStagingBuffer(Device& device, Buffer& stagingBuffer, uint32 size);


        MetalBuffer(const MetalBuffer&) = delete;
        MetalBuffer& operator=(const MetalBuffer&) = delete;
        MetalBuffer(MetalBuffer&& other) noexcept;
        MetalBuffer& operator=(MetalBuffer&& other) noexcept;

    private:

        /**
         * @brief Creates a buffer allocated with at least the given length
         * @param outBuffer The address to write the new buffer to
         * @param bufferLength The length of the buffer
         */
        void CreateBuffer(MTL::Buffer** outBuffer, uint32 bufferLength);

        /**
         * @brief Destroys the buffer
         * @param buffer The buffer to destroy
         */
        void DestroyBuffer(MTL::Buffer* buffer);


        MTL::Device* m_Device;
        MTL::Buffer* m_Buffer;

        uint32 m_BufferLength;
        uint32 m_UsedMemorySize;
        GPUMemoryType m_MemoryType;
    };

}
