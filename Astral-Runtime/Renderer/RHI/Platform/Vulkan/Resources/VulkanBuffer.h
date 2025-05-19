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

        void CopyDataIn(VulkanDevice& device, VulkanBuffer& sourceBuffer, VkDeviceSize size);

        void* GetNativeHandle() override { return m_Buffer; }

    private:

        void CreateBuffer();
        void DestroyBuffer();

        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);

        VkDevice m_Device;
        VkBufferUsageFlags m_Usage;
        uint32 m_Size;
        VkPhysicalDeviceMemoryProperties m_DeviceMemoryProperties;
        VkMemoryPropertyFlags m_RequestedPropertyFlags;


        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
        VkDeviceSize m_DeviceSize;
    };

}
