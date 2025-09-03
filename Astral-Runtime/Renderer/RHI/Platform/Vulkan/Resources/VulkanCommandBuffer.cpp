/**
* @file VulkanCommandBuffer.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanCommandBuffer.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

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

        m_BoundPipeline = nullptr;
        m_BoundDescriptorSets.clear();
    }


    void VulkanCommandBuffer::BeginRecording()
    {
        m_State = State::RECORDING;

        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
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

        m_BoundPipeline = nullptr;
        m_BoundDescriptorSets.clear();
    }


    void VulkanCommandBuffer::BindPipeline(PipelineStateHandle pipeline)
    {
        if (!pipeline)
        {
            WARN("Tried to bind pipeline handle that was null!");
            return;
        }

        if (pipeline->GetHandleHandle() == m_BoundPipeline->GetHandleHandle()) { return; } // Prevent redundant pipeline bind call
        if (pipeline->GetDescriptorSetLayout() == m_BoundPipeline->GetDescriptorSetLayout()) { m_BoundDescriptorSets.clear(); }

        VkPipeline vkPipeline = (VkPipeline)pipeline->GetHandleHandle();
        VkPipelineBindPoint bindPoint = pipeline->GetPipelineType() == PipelineType::GRAPHICS ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
        vkCmdBindPipeline(m_CommandBuffer, bindPoint, vkPipeline);
        m_BoundPipeline = pipeline;
    }


    void VulkanCommandBuffer::BindDescriptorSet(DescriptorSetHandle descriptorSet, uint32 binding)
    {
        if (!descriptorSet)
        {
            WARN("Tried to bind descriptor set handle that was null!");
            return;
        }

        if (!m_BoundPipeline)
        {
            WARN("Tried to bind descriptor set handle with no bound pipeline!");
            return;
        }

        if (m_BoundDescriptorSets.size() <= binding) { m_BoundDescriptorSets.resize(binding + 1); }
        if (m_BoundDescriptorSets[binding] && m_BoundDescriptorSets[binding]->GetNativeHandle() == descriptorSet->GetNativeHandle()) { return; } // Prevent redundant descriptor set bind call

        VkDescriptorSet vkDescriptorSet = (VkDescriptorSet)descriptorSet->GetNativeHandle();
        VkPipelineLayout vkPipelineLayout = (VkPipelineLayout)m_BoundPipeline->GetPipelineLayout();
        VkPipelineBindPoint bindPoint = m_BoundPipeline->GetPipelineType() == PipelineType::GRAPHICS ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
        vkCmdBindDescriptorSets(m_CommandBuffer, bindPoint, vkPipelineLayout,
                    binding, 1, &vkDescriptorSet, 0, nullptr);

        m_BoundDescriptorSets.resize(binding + 1); // Remove all bound descriptor sets above this binding
        m_BoundDescriptorSets[binding] = descriptorSet;
    }


    void VulkanCommandBuffer::SetViewportAndScissor(UVec2 dimensions)
    {
        if (!m_BoundPipeline)
        {
            WARN("Tried to set viewport and scissor with no bound pipeline!");
            return;
        }

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)dimensions.x,
            .height = (float)dimensions.y,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };
        vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

        VkRect2D rect = {
            .offset = {0, 0},
            .extent = {dimensions.x, dimensions.y},
        };
        vkCmdSetScissor(m_CommandBuffer, 0, 1, &rect);
    }


    void VulkanCommandBuffer::BeginRenderPass(FramebufferHandle frameBufferHandle)
    {

    }


    void VulkanCommandBuffer::NextSubpass()
    {

    }


    void VulkanCommandBuffer::EndRenderPass()
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
