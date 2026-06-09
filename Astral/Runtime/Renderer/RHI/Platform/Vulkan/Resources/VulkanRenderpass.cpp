/**
* @file VulkanRenderpass.cpp
* @author Andrew Fagan
* @date 5/16/25
*/

#include "VulkanRenderpass.h"

#include "Core/Utilities/Asserts.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

namespace Astral {

    VulkanRenderPass::VulkanRenderPass(const VulkanRenderpassDesc& desc)
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
        m_AttachmentDescriptions.push_back(attachmentDescription);
        return m_AttachmentDescriptions.size() - 1;
    }


    void VulkanRenderPass::AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_ColorAttachments.push_back(attachmentReference);
    }


    void VulkanRenderPass::AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_ResolveAttachments.push_back(attachmentReference);
    }


    void VulkanRenderPass::AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_DepthStencilAttachment = attachmentReference;
    }


    void VulkanRenderPass::EndBuildingRenderPass()
    {
        // CreateRenderPass();
    }


    void VulkanRenderPass::Invalidate()
    {

    }


    AttachmentDescription VulkanRenderPass::GetAttachmentDescription(AttachmentIndex attachmentIndex) const
    {
        ASSERT(attachmentIndex >= 0 && attachmentIndex < m_AttachmentDescriptions.size(), "The given attachment index does not fit the bounds of the attachment descriptions vector!")
        return m_AttachmentDescriptions[attachmentIndex];
    }


    uint32 VulkanRenderPass::GetNumAttachments() const
    {
        return m_AttachmentDescriptions.size();
    }


    const std::vector<AttachmentReference>& VulkanRenderPass::GetColorAttachmentReferences() const
    {
        return m_ColorAttachments;
    }


    const std::vector<AttachmentReference>& VulkanRenderPass::GetResolveAttachmentReferences() const
    {
        return m_ResolveAttachments;
    }


    AttachmentReference VulkanRenderPass::GetDepthStencilAttachmentReference() const
    {
        return m_DepthStencilAttachment;
    }

}