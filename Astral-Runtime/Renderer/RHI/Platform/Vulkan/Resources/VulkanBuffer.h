/**
* @file VulkanBuffer.h
* @author Andrew Fagan
* @date 5/17/2025
*/


#pragma once

#include "Renderer/RHI/Resources/Buffer.h"

#include <vulkan/vulkan_core.h>

#include "VulkanDevice.h"

namespace Astral {

    struct VulkanBufferDesc
    {
        VkDevice Device;
        VkBufferUsageFlags Usage;
        uint32 Size;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
        VkMemoryPropertyFlags RequestedMemoryPropertyFlags;
    };

    class VulkanBuffer : public Buffer
    {
    public:
        VulkanBuffer() = default;
        explicit VulkanBuffer(const VulkanBufferDesc& desc);
        ~VulkanBuffer() override;

        [[nodiscard]] VkDeviceMemory GetDeviceMemory() const { return m_Memory; }
        [[nodiscard]] VkDeviceSize GetDeviceSize() const { return m_DeviceSize; }
        void MapPointer(void** cpuPtr) override;
        void UnmapPointer() override;
        void CopyDataToBuffer(void* data, uint32 size) override;

        void CopyFromStagingBuffer(VulkanDevice& device, VulkanBuffer& sourceBuffer, VkDeviceSize size);

        uint32 GetUsedSize() override { return m_Size; }

        void* GetNativeHandle() override { return m_Buffer; }


        VulkanBuffer(const VulkanBuffer&) = delete;
        VulkanBuffer& operator=(const VulkanBufferDesc& desc) = delete;
        VulkanBuffer(VulkanBuffer&& other) = delete;
        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    private:


        void CreateBuffer();
        void AllocateMemory();

        void DestroyBuffer();
        void FreeMemory();


        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);

        VkDevice m_Device;
        VkBufferUsageFlags m_Usage;
        uint32 m_Size;
        VkPhysicalDeviceMemoryProperties m_DeviceMemoryProperties;
        VkMemoryPropertyFlags m_RequestedPropertyFlags;


        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
        VkDeviceSize m_DeviceSize;
        bool m_IsDeviceMemoryMapped;
    };

}
