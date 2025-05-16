/**
* @file VulkanRenderpass.cpp
* @author Andrew Fagan
* @date 5/16/25
*/

#include "VulkanRenderpass.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    void VulkanRenderpass::BeginRenderpass(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkClearValue clearColorValues = {{0.0f, 0.0f, 1.0f, 1.0f}};

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = m_Renderpass,
            .renderArea = {
                .offset = {0,0},
                .extent = {}
            },
            .clearValueCount = 1,
            .pClearValues = &clearColorValues
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }


    void VulkanRenderpass::EndRenderpass(CommandBufferHandle commandBufferHandle)
    {

    }


    void VulkanRenderpass::CreateRenderPass()
    {
        VkAttachmentDescription attachDesc = {
            .flags = 0,
            .format = m_SwapchainSurfaceFormat.format,
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
            .layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
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

        VkResult result = vkCreateRenderPass(m_Device, &createInfo, nullptr, &m_Renderpass);
        ASSERT(result == VK_SUCCESS, "Renderpass failed to create!");
    }


    void VulkanRenderpass::DestoryRenderpass()
    {
        vkDestroyRenderPass(m_Device, m_Renderpass, nullptr);
    }

}
