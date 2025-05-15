/**
* @file VulkanCommandBuffer.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanCommandBuffer.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanCommandBuffer::VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer) :
        m_Device(device),
        m_CommandPool(commandPool),
        m_CommandBuffer(commandBuffer)
    {
        m_IsValid = true;
        m_State = State::EMPTY;
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &m_CommandBuffer);
    }


    void VulkanCommandBuffer::BeginRecording()
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
        };

        VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &commandBufferBeginInfo);
        ASSERT(result == VK_SUCCESS, "Command buffer failed to begin recording!");
    }


    void VulkanCommandBuffer::EndRecording()
    {
        vkEndCommandBuffer(m_CommandBuffer);
    }


    void VulkanCommandBuffer::Reset()
    {
        vkResetCommandBuffer(m_CommandBuffer, 0);
    }


    void VulkanCommandBuffer::BindPipeline()
    {

    }


    void VulkanCommandBuffer::BindDescriptorSet()
    {

    }

}
