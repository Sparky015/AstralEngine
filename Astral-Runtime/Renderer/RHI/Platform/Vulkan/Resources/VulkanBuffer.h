/**
* @file VulkanBuffer.h
* @author Andrew Fagan
* @date 5/17/2025
*/


#pragma once

#include "Renderer/RHI/Resources/Buffer.h"

#include <vulkan/vulkan_core.h>

#include "VulkanPhysicalDevices.h"

namespace Astral {

    struct VulkanBufferDesc
    {
        VkDevice Device;
        VkBufferUsageFlags Usage;
        uint32 Size;
        VulkanPhysicalDevice& PhysicalDevice;
        VkMemoryPropertyFlags PropertyFlags;
    };

    class VulkanBuffer : public Buffer
    {
    public:
        VulkanBuffer(const VulkanBufferDesc& desc);
        ~VulkanBuffer() override;

    private:

        void CreateBuffer();
        void DestroyBuffer();

        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);

        VkDevice m_Device;
        VkBufferUsageFlags m_Usage;
        uint32 m_Size;
        VulkanPhysicalDevice& m_PhysicalDevice;
        VkMemoryPropertyFlags m_PropertyFlags;


        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
        VkDeviceSize m_DeviceSize;
    };

}
