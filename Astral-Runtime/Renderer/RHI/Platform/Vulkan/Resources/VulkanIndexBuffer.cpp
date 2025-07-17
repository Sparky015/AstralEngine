/**
* @file VulkanIndexBuffer.cpp
* @author Andrew Fagan
* @date 5/21/2025
*/

#include "VulkanIndexBuffer.h"

namespace Astral {

    VulkanIndexBuffer::VulkanIndexBuffer(const VulkanIndexBufferDesc& desc) :
        m_Device(desc.Device),
        m_IndexBuffer(),
        m_SizeInBytes(desc.SizeInBytes),
        m_IndiceData(desc.IndiceData)
    {
        CreateIndexBuffer(desc);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {

    }


    void VulkanIndexBuffer::Bind(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkBuffer indexBuffer = (VkBuffer)m_IndexBuffer.GetNativeHandle();
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }


    void VulkanIndexBuffer::CreateIndexBuffer(const VulkanIndexBufferDesc& desc)
    {
        VulkanBufferDesc stagingBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .Size = desc.SizeInBytes,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        VulkanBuffer stagingBuffer = VulkanBuffer(stagingBufferDesc);
        stagingBuffer.CopyDataToBuffer(m_IndiceData, desc.SizeInBytes);


        VulkanBufferDesc indexBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            .Size = desc.SizeInBytes,
            .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        };

        m_IndexBuffer = VulkanBuffer{indexBufferDesc};

        m_IndexBuffer.CopyFromStagingBuffer(desc.VulkanDevice, stagingBuffer, desc.SizeInBytes);
    }

}
