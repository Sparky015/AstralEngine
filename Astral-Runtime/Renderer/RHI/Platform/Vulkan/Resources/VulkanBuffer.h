/**
* @file VulkanBuffer.h
* @author Andrew Fagan
* @date 5/17/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Buffer.h"

#include "Renderer/RHI/Common/MemoryTypes.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
     * @brief A description of how to build a Vulkan buffer
     */
    struct VulkanBufferDesc
    {
        VkDevice Device;
        VkBufferUsageFlags Usage;
        uint32 Size;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
        VkMemoryPropertyFlags RequestedMemoryPropertyFlags;
    };


    /**
     * @brief A wrapper around a Vulkan buffer supporting extra convenience functions
     */
    class VulkanBuffer : public Buffer
    {
    public:
        VulkanBuffer() = default;
        explicit VulkanBuffer(const VulkanBufferDesc& desc);
        ~VulkanBuffer() override;

        /**
         * @brief Gets the amount of memory allocated to the buffer
         * @return The amount of memory used in the buffer
         */
        uint32 GetAllocatedSize() override;

        /**
         * @brief Gets the amount of memory written to in the buffer
         * @return The amount of memory used in the buffer
         * @warning This is possible to be inaccurate if writing to a MapPointer was done
         */
        uint32 GetUsedSize() override;

        /**
         * @brief Reallocates the length of a buffer by creating a new buffer and copying data over
         * @param newSize The length to make the new buffer
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer
         */
        void ReallocateMemory(uint32 newSize) override;

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
         * @note This is only supported by host visible buffers. This does not support uploading to a device local buffer. Reallocation is
         * skipped if the new buffer size is less than or equal to the current buffer size.
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
         * @brief Gets the native handle of the buffer
         * @return The native handle of the buffer (VkBuffer)
        */
        void* GetNativeHandle() override;


        VulkanBuffer(const VulkanBuffer&) = delete;
        VulkanBuffer& operator=(const VulkanBuffer& desc) = delete;
        VulkanBuffer(VulkanBuffer&& other) noexcept;
        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    protected:

        /**
        * @brief Copies data from a staging buffer to this buffer using a copy buffer command
        * @param stagingBuffer The origin buffer to copy from
        * @param size The size of the data that should be copied to this buffer
        * @warning This forces a blocking wait while the GPU transfers the data.
        */
        void CopyFromStagingBuffer(Buffer& stagingBuffer, uint32 size) override;

    private:

        /**
         * @brief Creates a buffer allocated with at least the given length
         * @param outBuffer The address to write the new buffer to
         * @param length The length of the buffer
         */
        void CreateBuffer(VkBuffer* outBuffer, uint32 length);

        /**
         * @brief Allocates the backing memory for the buffer and binds the memory to the buffer
         * @param outDeviceMemory The address to write the device memory handle to
         * @param buffer The buffer to allocate memory for
         */
        uint32 AllocateMemory(VkDeviceMemory* outDeviceMemory, VkBuffer buffer);

        /**
         * @brief Destroys the buffer
         * @param buffer The buffer to destroy
         */
        void DestroyBuffer(VkBuffer buffer);

        /**
         * @brief Frees the backing memory of the buffer
         * @param deviceMemory The device memory to free
         */
        void FreeMemory(VkDeviceMemory deviceMemory);

        /**
         * @brief Finds a memory type with the flags requested by the buffer
         * @param memoryTypeBitsMask The flags requested by the buffer
         * @return The index of the memory type that has the requested flags by the buffer
         */
        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);

        VkDevice m_Device;
        VkBufferUsageFlags m_Usage;
        uint32 m_UsedMemorySize;
        VkPhysicalDeviceMemoryProperties m_DeviceMemoryProperties;
        VkMemoryPropertyFlags m_RequestedPropertyFlags;

        VkBuffer m_Buffer;
        VkDeviceMemory m_BufferMemory;
        VkDeviceSize m_BufferDeviceSize;
        bool m_IsDeviceMemoryMapped;
        GPUMemoryType m_MemoryType;
    };

}
