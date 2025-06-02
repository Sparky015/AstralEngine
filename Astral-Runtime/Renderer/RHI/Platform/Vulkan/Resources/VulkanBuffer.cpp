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
        m_PrimaryBuffer(),
        m_PrimaryMemory(),
        m_PrimaryDeviceSize(),
        m_IsDeviceMemoryMapped(false)
    {
        CreateBuffer();
        AllocateMemory();
    }

    VulkanBuffer::~VulkanBuffer()
    {
        FreeMemory();
        DestroyBuffer();
    }


    void VulkanBuffer::MapPointer(void** cpuPtr)
    {
        if (m_IsDeviceMemoryMapped) { vkUnmapMemory(m_Device, m_PrimaryMemory); }

        VkResult result = vkMapMemory(m_Device, m_PrimaryMemory, 0,
                                        m_PrimaryDeviceSize, 0, cpuPtr);
        ASSERT(result == VK_SUCCESS, "Failed to map memory in buffer")

        m_IsDeviceMemoryMapped = true;
    }


    void VulkanBuffer::UnmapPointer()
    {
        if (m_IsDeviceMemoryMapped)
        {
            vkUnmapMemory(m_Device, m_PrimaryMemory);
            m_IsDeviceMemoryMapped = false;
        }
    }


    void VulkanBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        if (!data) { return; }

        void* memory = nullptr;
        ASSERT(size <= m_PrimaryDeviceSize, "Data does not fit in buffer!")
        MapPointer(&memory);
        memcpy(memory, data, size);
        UnmapPointer();
    }


    void VulkanBuffer::CopyFromStagingBuffer(VulkanDevice& device, VulkanBuffer& sourceBuffer, VkDeviceSize size)
    {
        ASSERT(size <= m_PrimaryDeviceSize, "Data does not fit in buffer!")
        VkBuffer stagingBuffer = (VkBuffer)sourceBuffer.GetNativeHandle();

        CommandBufferHandle commandBufferHandle = device.AllocateCommandBuffer();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkBufferCopy bufferCopy = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };

        commandBufferHandle->BeginRecording();
        vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_PrimaryBuffer, 1, &bufferCopy);
        commandBufferHandle->EndRecording();

        CommandQueueHandle commandQueueHandle = device.GetCommandQueue();
        commandQueueHandle->SubmitSync(commandBufferHandle);
        commandQueueHandle->WaitIdle();
    }


    VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
    {
        m_Device = other.m_Device;
        m_PrimaryBuffer = other.m_PrimaryBuffer;
        m_PrimaryMemory = other.m_PrimaryMemory;
        m_PrimaryDeviceSize = other.m_PrimaryDeviceSize;
        m_IsDeviceMemoryMapped = other.m_IsDeviceMemoryMapped;

        other.m_PrimaryBuffer = VK_NULL_HANDLE;
        other.m_PrimaryMemory = VK_NULL_HANDLE;
        other.m_PrimaryDeviceSize = 0;
        other.m_IsDeviceMemoryMapped = false;


        return *this;
    }


    void VulkanBuffer::CreateBuffer()
    {
        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = m_Size,
            .usage = m_Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        VkResult result = vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_PrimaryBuffer);
        ASSERT(result == VK_SUCCESS, "Failed to create buffer!");
    }


    void VulkanBuffer::AllocateMemory()
    {
        VkMemoryRequirements memoryRequirements = {};
        vkGetBufferMemoryRequirements(m_Device, m_PrimaryBuffer, &memoryRequirements);

        m_PrimaryDeviceSize = memoryRequirements.size;

        uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits);

        VkMemoryAllocateInfo memoryAllocationInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = memoryTypeIndex
        };

        VkResult result = vkAllocateMemory(m_Device, &memoryAllocationInfo, nullptr, &m_PrimaryMemory);
        ASSERT(result == VK_SUCCESS, "Failed to allocate memory!");

        result = vkBindBufferMemory(m_Device, m_PrimaryBuffer, m_PrimaryMemory, 0);
        ASSERT(result == VK_SUCCESS, "Failed to bind buffer memory!");
    }


    void VulkanBuffer::DestroyBuffer()
    {
        vkDestroyBuffer(m_Device, m_PrimaryBuffer, nullptr);
    }


    void VulkanBuffer::FreeMemory()
    {
        vkFreeMemory(m_Device, m_PrimaryMemory, nullptr);
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
