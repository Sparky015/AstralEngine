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
        m_DeviceMemoryProperties(desc.DeviceMemoryProperties),
        m_RequestedPropertyFlags(desc.RequestedMemoryPropertyFlags),
        m_Buffer(),
        m_Memory(),
        m_DeviceSize(),
        m_IsDeviceMemoryMapped(false)
    {
        CreateFinalBuffer();
    }

    VulkanBuffer::~VulkanBuffer()
    {
        DestroyFinalBuffer();
    }


    void VulkanBuffer::MapPointer(void** cpuPtr)
    {
        if (m_IsDeviceMemoryMapped) { vkUnmapMemory(m_Device, m_Memory); }

        VkResult result = vkMapMemory(m_Device, m_Memory, 0,
                                        m_DeviceSize, 0, cpuPtr);
        ASSERT(result == VK_SUCCESS, "Failed to map memory in buffer")

        m_IsDeviceMemoryMapped = true;
    }


    void VulkanBuffer::UnmapPointer()
    {
        if (m_IsDeviceMemoryMapped)
        {
            vkUnmapMemory(m_Device, m_Memory);
            m_IsDeviceMemoryMapped = false;
        }
    }


    void VulkanBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        void* memory = nullptr;
        ASSERT(size <= m_DeviceSize, "Data does not fit in buffer!")
        MapPointer(&memory);
        memcpy(memory, data, size);
        UnmapPointer();
    }


    void VulkanBuffer::CopyFromStagingBuffer(VulkanDevice& device, VulkanBuffer& sourceBuffer, VkDeviceSize size)
    {
        ASSERT(size <= m_DeviceSize, "Data does not fit in buffer!")
        VkBuffer stagingBuffer = (VkBuffer)sourceBuffer.GetNativeHandle();

        CommandBufferHandle commandBufferHandle = device.AllocateCommandBuffer();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkBufferCopy bufferCopy = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };

        commandBufferHandle->BeginRecording();
        vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &bufferCopy);
        commandBufferHandle->EndRecording();

        CommandQueueHandle commandQueueHandle = device.GetCommandQueue();
        commandQueueHandle->SubmitSync(commandBufferHandle);
        commandQueueHandle->WaitIdle();
    }


    void VulkanBuffer::CreateFinalBuffer()
    {
        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = m_Size,
            .usage = m_Usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
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


    void VulkanBuffer::DestroyFinalBuffer()
    {
        vkFreeMemory(m_Device, m_Memory, nullptr);
        vkDestroyBuffer(m_Device, m_Buffer, nullptr);
    }


    uint32 VulkanBuffer::GetMemoryTypeIndex(uint32 memoryTypeBitsMask)
    {
        for (uint32 i = 0; i < m_DeviceMemoryProperties.memoryTypeCount; i++)
        {
            const VkMemoryType& memoryType = m_DeviceMemoryProperties.memoryTypes[i];
            uint32 currentBitmask = (1 << i);
            bool isCurrentMemoryTypeSupported = (memoryTypeBitsMask & currentBitmask);
            bool hasRequiredMemoryProperties = ((memoryType.propertyFlags & m_RequestedPropertyFlags) == m_RequestedPropertyFlags);
            if (isCurrentMemoryTypeSupported && hasRequiredMemoryProperties) { return i; }
        }

        ASTRAL_ERROR("Failed to find a suitable memory type for Buffer object!");
    }

}
