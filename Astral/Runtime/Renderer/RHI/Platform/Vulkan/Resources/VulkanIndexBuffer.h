/**
* @file VulkanIndexBuffer.h
* @author Andrew Fagan
* @date 5/21/2025
*/

#pragma once

#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "Renderer/RHI/Common/MemoryTypes.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"

namespace Astral {

    /**
     * @brief A description of how to build a Vulkan index buffer
     */
    struct VulkanIndexBufferDesc
    {
        VkDevice Device;
        uint32* IndexData;
        uint32 DataSize;
        GPUMemoryType MemoryType;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
    };

    /**
     * @brief A wrapper around a Vulkan buffer supporting extra convenience functions for an index buffer
     */
    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        explicit VulkanIndexBuffer(const VulkanIndexBufferDesc& desc);
        ~VulkanIndexBuffer() override;

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
         */
        void UnmapPointer() override;

        /**
         * @brief Copies data to this buffer
         * @param data The address of the data to copy to the buffer
         * @param size The size of the data to copy to this buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer.
         */
        void CopyDataToBuffer(void* data, uint32 size) override;

        /**
         * @brief Uploads the given data to this buffer
         * @param data The address of the data to copy to the buffer
         * @param size The size of the data to upload to this buffer
         * @note This is only supported by device local buffers. This does not support copying to a host visible buffer.
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
         * @return The native handle of the buffer (VkBuffer)
         */
        void* GetNativeHandle() override;

        VulkanIndexBuffer(const VulkanIndexBuffer&) = delete;
        VulkanIndexBuffer& operator=(const VulkanIndexBuffer& desc) = delete;
        VulkanIndexBuffer(VulkanIndexBuffer&& other) noexcept;
        VulkanIndexBuffer& operator=(VulkanIndexBuffer&& other) noexcept;

    private:

        /**
         * @brief Creates a Vulkan index buffer from a Vulkan index buffer description
         * @param desc The description of the Vulkan index buffer to build
         */
        void CreateIndexBuffer(const VulkanIndexBufferDesc& desc);

        VulkanBuffer m_IndexBuffer;
        uint32 m_DataSize;
    };


}
