/**
* @file VulkanRenderpass.cpp
* @author Andrew Fagan
* @date 5/16/25
*/

#include "VulkanRenderpass.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanRenderPass::VulkanRenderPass(const VulkanRenderpassDesc& desc) :
        m_Device(desc.Device),
        m_Format(desc.Format)
    {
        CreateRenderPass();
    }


    VulkanRenderPass::~VulkanRenderPass()
    {
        DestroyRenderPass();
    }


    void VulkanRenderPass::BeginRenderPass(CommandBufferHandle commandBufferHandle, FramebufferHandle frameBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkClearValue clearColorValues = {.color = {0.0f, 0.0f, 1.0f, 1.0f}};
        VkFramebuffer framebuffer = (VkFramebuffer)frameBufferHandle->GetNativeHandle();
        UVec2 extent = frameBufferHandle->GetExtent();

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = m_RenderPass,
            .framebuffer = framebuffer,
            .renderArea = {
                .offset = {0,0},
                .extent = {extent.x, extent.y}
            },
            .clearValueCount = 1,
            .pClearValues = &clearColorValues,
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }


    void VulkanRenderPass::NextSubpass(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
    }


    void VulkanRenderPass::EndRenderPass(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        vkCmdEndRenderPass(commandBuffer);
    }


    void VulkanRenderPass::CreateRenderPass()
    {
        VkAttachmentDescription attachDesc = {
            .flags = 0,
            .format = m_Format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference attachRef = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpassDesc = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &attachRef,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };

        VkRenderPassCreateInfo renderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .attachmentCount = 1,
            .pAttachments = &attachDesc,
            .subpassCount = 1,
            .pSubpasses = &subpassDesc,
            .dependencyCount = 0,
            .pDependencies = nullptr
        };

        VkResult result = vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass);
        ASSERT(result == VK_SUCCESS, "Renderpass failed to create!");
    }


    void VulkanRenderPass::DestroyRenderPass()
    {
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
    }

}
