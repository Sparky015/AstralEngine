/**
* @file VulkanBuffer.cpp
* @author Andrew Fagan
* @date 5/17/2025
*/

#include "VulkanBuffer.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    VulkanBuffer::VulkanBuffer(const VulkanBufferDesc& desc) :
        m_Device(desc.Device),
        m_Usage(desc.Usage),
        m_Size(desc.Size),
        m_PhysicalDevice(desc.PhysicalDevice),
        m_PropertyFlags(desc.PropertyFlags),
        m_Buffer(),
        m_Memory(),
        m_DeviceSize()
    {

    }

    VulkanBuffer::~VulkanBuffer()
    {

    }


    void VulkanBuffer::CreateBuffer()
    {
        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = m_Size,
            .usage = m_Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        VkResult result = vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_Buffer);
        ASSERT(result == VK_SUCCESS, "Failed to create buffer!");

        VkMemoryRequirements memoryRequirements = {};
        vkGetBufferMemoryRequirements(m_Device, m_Buffer, &memoryRequirements);

        m_DeviceSize = memoryRequirements.size;

        uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits);

        VkMemoryAllocateInfo memoryAllocationInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = memoryTypeIndex
        };

        result = vkAllocateMemory(m_Device, &memoryAllocationInfo, nullptr, &m_Memory);
        ASSERT(result == VK_SUCCESS, "Failed to allocate memory!");

        result = vkBindBufferMemory(m_Device, m_Buffer, m_Memory, 0);
        ASSERT(result == VK_SUCCESS, "Failed to bind buffer memory!");
    }


    void VulkanBuffer::DestroyBuffer()
    {
        vkDestroyBuffer(m_Device, m_Buffer, nullptr);
    }


    uint32 VulkanBuffer::GetMemoryTypeIndex(uint32 memoryTypeBitsMask)
    {
        const VkPhysicalDeviceMemoryProperties& memoryProperties = m_PhysicalDevice.memoryProperties;
        for (uint32 i = 0; i < memoryProperties.memoryTypeCount; i++)
        {
            const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
            uint32 currentBitmask = (1 << i);
            bool isCurrentMemoryTypeSupported = (memoryTypeBitsMask & currentBitmask);
            bool hasRequiredMemoryProperties = ((memoryType.propertyFlags & m_PropertyFlags) == m_PropertyFlags);
            if (isCurrentMemoryTypeSupported && hasRequiredMemoryProperties) { return i; }
        }

        ASTRAL_ERROR("Failed to find a suitable memory type for Buffer object!");
    }

}
