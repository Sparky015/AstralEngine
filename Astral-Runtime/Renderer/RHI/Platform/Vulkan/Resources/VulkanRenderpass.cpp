/**
* @file VulkanRenderpass.cpp
* @author Andrew Fagan
* @date 5/16/25
*/

#include "VulkanRenderpass.h"

#include "Debug/Utilities/Asserts.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

namespace Astral {

    VulkanRenderPass::VulkanRenderPass(const VulkanRenderpassDesc& desc) :
        m_Device(desc.Device),
        m_RenderPass(VK_NULL_HANDLE)
    {

    }


    VulkanRenderPass::~VulkanRenderPass()
    {
        VulkanRenderPass::Invalidate();
    }


    void VulkanRenderPass::BeginBuildingRenderPass()
    {
        Invalidate();
    }


    AttachmentIndex VulkanRenderPass::DefineAttachment(const AttachmentDescription& attachmentDescription)
    {
        VkFormat format = ConvertImageFormatToVkFormat(attachmentDescription.Format);
        VkImageLayout initialLayout = ConvertImageLayoutToVkImageLayout(attachmentDescription.InitialLayout);
        VkImageLayout finalLayout = ConvertImageLayoutToVkImageLayout(attachmentDescription.FinalLayout);
        VkAttachmentLoadOp loadOp = ConvertToVkLoadOp(attachmentDescription.LoadOp);
        VkAttachmentStoreOp storeOp = ConvertToVkStoreOp(attachmentDescription.StoreOp);

        VkAttachmentDescription vkAttachmentDescription = {
            .flags = 0,
            .format = format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = loadOp,
            .storeOp = storeOp,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = initialLayout,
            .finalLayout = finalLayout
        };

        m_RenderPassAttachments.push_back(vkAttachmentDescription);
        return m_RenderPassAttachments.size() - 1;
    }


    void VulkanRenderPass::BeginBuildingSubpass()
    {
        m_SubpassAttachments.push_back(SubpassAttachments()); // Only create new attachments struct, desc will be made later
    }


    void VulkanRenderPass::AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        VkImageLayout imageLayout = ConvertImageLayoutToVkImageLayout(optimalImageLayout);
        VkAttachmentReference attachmentReference = {
            .attachment = attachmentIndex,
            .layout = imageLayout,
        };

        m_SubpassAttachments.back().InputAttachments.push_back(attachmentReference);
    }


    void VulkanRenderPass::AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        VkImageLayout imageLayout = ConvertImageLayoutToVkImageLayout(optimalImageLayout);
        VkAttachmentReference attachmentReference = {
            .attachment = attachmentIndex,
            .layout = imageLayout,
        };

        m_SubpassAttachments.back().ColorAttachments.push_back(attachmentReference);
    }


    void VulkanRenderPass::AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        VkImageLayout imageLayout = ConvertImageLayoutToVkImageLayout(optimalImageLayout);
        VkAttachmentReference attachmentReference = {
            .attachment = attachmentIndex,
            .layout = imageLayout,
        };

        m_SubpassAttachments.back().ResolveAttachments.push_back(attachmentReference);
    }


    void VulkanRenderPass::AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        VkImageLayout imageLayout = ConvertImageLayoutToVkImageLayout(optimalImageLayout);
        VkAttachmentReference attachmentReference = {
            .attachment = attachmentIndex,
            .layout = imageLayout,
        };

        m_SubpassAttachments.back().DepthStencilAttachment.push_back(attachmentReference);
    }


    void VulkanRenderPass::PreserveAttachment(AttachmentIndex attachmentIndex)
    {
        m_SubpassAttachments.back().PreserveAttachments.push_back(attachmentIndex);
    }


    SubpassIndex VulkanRenderPass::EndBuildingSubpass()
    {
        SubpassAttachments& subpassAttachments = m_SubpassAttachments.back();

        VkSubpassDescription subpassDescription = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = (uint32)subpassAttachments.InputAttachments.size(),
            .pInputAttachments = subpassAttachments.InputAttachments.data(),
            .colorAttachmentCount = (uint32)subpassAttachments.ColorAttachments.size(),
            .pColorAttachments = subpassAttachments.ColorAttachments.data(),
            .pResolveAttachments = subpassAttachments.ResolveAttachments.data(),
            .pDepthStencilAttachment = subpassAttachments.DepthStencilAttachment.data(),
            .preserveAttachmentCount = (uint32)subpassAttachments.PreserveAttachments.size(),
            .pPreserveAttachments = (uint32*)subpassAttachments.PreserveAttachments.data(),
        };

        m_SubpassDescriptions.push_back(subpassDescription);
        return m_SubpassDescriptions.size() - 1;
    }


    void VulkanRenderPass::DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks)
    {
        if (sourceSubpass == SubpassExternal) { sourceSubpass = VK_SUBPASS_EXTERNAL; }
        if (destinationSubpass == SubpassExternal) { destinationSubpass = VK_SUBPASS_EXTERNAL; }

        VkPipelineStageFlags sourceStageMask = ConvertPipelineStageToVkPipelineStageFlags(subpassDependencyMasks.SourceStageMask);
        VkPipelineStageFlags destinationStageMask = ConvertPipelineStageToVkPipelineStageFlags(subpassDependencyMasks.DestinationStageMask);
        VkAccessFlags sourceAccessMask = ConvertAccessFlagsToVkAccessFlags(subpassDependencyMasks.SourceAccessMask);
        VkAccessFlags destinationAccessMask = ConvertAccessFlagsToVkAccessFlags(subpassDependencyMasks.DestinationAccessMask);

        VkSubpassDependency subpassDependency = {
            .srcSubpass = sourceSubpass,
            .dstSubpass = destinationSubpass,
            .srcStageMask = sourceStageMask,
            .dstStageMask = destinationStageMask,
            .srcAccessMask = sourceAccessMask,
            .dstAccessMask = destinationAccessMask,
            .dependencyFlags = 0
        };

        m_SubpassDependencies.push_back(subpassDependency);
    }


    void VulkanRenderPass::EndBuildingRenderPass()
    {
        CreateRenderPass();
    }


    void VulkanRenderPass::Invalidate()
    {
        if (m_RenderPass != VK_NULL_HANDLE)
        {
            DestroyRenderPass();
        }
        m_RenderPassAttachments.clear();
        m_SubpassAttachments.clear();
        m_SubpassDescriptions.clear();
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
        VkRenderPassCreateInfo renderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .attachmentCount = (uint32)m_RenderPassAttachments.size(),
            .pAttachments = m_RenderPassAttachments.data(),
            .subpassCount = (uint32)m_SubpassDescriptions.size(),
            .pSubpasses = m_SubpassDescriptions.data(),
            .dependencyCount = (uint32)m_SubpassDependencies.size(),
            .pDependencies = m_SubpassDependencies.data()
        };

        VkResult result = vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass);
        ASSERT(result == VK_SUCCESS, "Renderpass failed to create!");
    }


    void VulkanRenderPass::DestroyRenderPass()
    {
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
        m_RenderPass = VK_NULL_HANDLE;
    }


    VkAttachmentLoadOp VulkanRenderPass::ConvertToVkLoadOp(AttachmentLoadOp loadOp)
    {
        switch (loadOp)
        {
            case AttachmentLoadOp::LOAD:   return VK_ATTACHMENT_LOAD_OP_LOAD;
            case AttachmentLoadOp::CLEAR:  return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case AttachmentLoadOp::DONT_CARE: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            default: ASTRAL_ERROR("Invalid Load Op Given!")
        }
    }


    VkAttachmentStoreOp VulkanRenderPass::ConvertToVkStoreOp(AttachmentStoreOp storeOp)
    {
        switch (storeOp)
        {
            case AttachmentStoreOp::STORE: return VK_ATTACHMENT_STORE_OP_STORE;
            case AttachmentStoreOp::DONT_CARE: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            default: ASTRAL_ERROR("Invalid Store Op Given!")
        }
    }

}
