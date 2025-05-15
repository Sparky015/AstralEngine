/**
* @file VulkanCommandBuffer.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanCommandBuffer.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBufferDesc& desc) :
        m_Device(desc.Device),
        m_CommandPool(desc.CommandPool),
        m_CommandBuffer(VK_NULL_HANDLE)
    {
        m_IsValid = true;
        m_State = State::EMPTY;
        AllocateCommandBuffer();
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        FreeCommandBuffer();
    }


    void VulkanCommandBuffer::BeginRecording()
    {
        m_State = State::RECORDING;

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
        m_State = State::RECORDED;
        vkEndCommandBuffer(m_CommandBuffer);
    }


    void VulkanCommandBuffer::Reset()
    {
        m_State = State::EMPTY;
        vkResetCommandBuffer(m_CommandBuffer, 0);
    }


    void VulkanCommandBuffer::BindPipeline()
    {

    }


    void VulkanCommandBuffer::BindDescriptorSet()
    {

    }


    void VulkanCommandBuffer::AllocateCommandBuffer()
    {
        VkCommandBufferAllocateInfo commandBufferAllocate = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkResult result = vkAllocateCommandBuffers(m_Device, &commandBufferAllocate, &m_CommandBuffer);
        ASSERT(result == VK_SUCCESS, "Vulkan command buffer(s) failed to allocate!");
    }


    void VulkanCommandBuffer::FreeCommandBuffer()
    {
        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &m_CommandBuffer);
    }

}
