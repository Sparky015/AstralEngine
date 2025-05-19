/**
* @file VulkanVertexBuffer.cpp
* @author Andrew Fagan
* @date 5/18/2025
*/

#include "VulkanVertexBuffer.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanVertexBuffer::VulkanVertexBuffer(const VulkanVertexBufferDesc& desc) :
        m_Device(desc.Device),
        m_VertexBuffer(),
        m_Size(desc.Size),
        m_VerticeData(desc.VerticeData)
    {

    }


    VulkanVertexBuffer::~VulkanVertexBuffer()
    {

    }


    void VulkanVertexBuffer::CreateVertexBuffer(const VulkanVertexBufferDesc& desc)
    {
        VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VulkanBufferDesc stagingBufferDesc = {
            .Device = m_Device,
            .Usage = usage,
            .Size = desc.Size,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = memoryProperties
        };
        VulkanBuffer stagingBuffer = VulkanBuffer(stagingBufferDesc);

        void* memory = nullptr;
        VkDeviceSize offset = 0;
        VkMemoryMapFlags flags = 0;
        VkResult result = vkMapMemory(m_Device, stagingBuffer.GetDeviceMemory(), offset,
                                        stagingBuffer.GetDeviceSize(), flags, &memory);
        ASSERT(result == VK_SUCCESS, "Failed to map memory in vertex buffer")

        memcpy(memory, desc.VerticeData, desc.Size);

        vkUnmapMemory(m_Device, stagingBuffer.GetDeviceMemory());

        VulkanBufferDesc vertexBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .Size = desc.Size,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        };
        VulkanBuffer vertexBuffer = VulkanBuffer(vertexBufferDesc);

        vertexBuffer.CopyDataIn(desc.VulkanDevice, stagingBuffer, desc.Size);
    }


    void VulkanVertexBuffer::DestroyVertexBuffer()
    {

    }

}
