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
        m_SizeInBytes(desc.SizeInBytes),
        m_VerticeData(desc.VerticeData)
    {
        CreateVertexBuffer(desc);
    }


    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
    }


    void VulkanVertexBuffer::Bind(CommandBufferHandle commandBufferHandle)
    {
        VkBuffer buffer = (VkBuffer)m_VertexBuffer.GetNativeHandle();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &buffer, offsets);
    }


    void VulkanVertexBuffer::CreateVertexBuffer(const VulkanVertexBufferDesc& desc)
    {
        VulkanBufferDesc stagingBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .Size = desc.SizeInBytes,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        VulkanBuffer stagingBuffer = VulkanBuffer(stagingBufferDesc);
        stagingBuffer.CopyDataToBuffer(desc.VerticeData, desc.SizeInBytes);


        VulkanBufferDesc vertexBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .Size = desc.SizeInBytes,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        };

        m_VertexBuffer = VulkanBuffer{vertexBufferDesc};

        m_VertexBuffer.CopyFromStagingBuffer(desc.VulkanDevice, stagingBuffer, desc.SizeInBytes);
    }

}
