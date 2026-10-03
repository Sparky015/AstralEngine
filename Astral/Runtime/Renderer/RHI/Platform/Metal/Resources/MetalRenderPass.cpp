/**
* @file MetalRenderPass.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalRenderPass.h"

#include "Core/Utilities/Asserts.h"

namespace Astral {

    MetalRenderPass::MetalRenderPass(const MetalRenderPassDesc& renderPassDesc)
    {

    }


    MetalRenderPass::~MetalRenderPass()
    {
        MetalRenderPass::Invalidate();
    }


    void MetalRenderPass::BeginBuildingRenderPass()
    {
        Invalidate();
    }


    AttachmentIndex MetalRenderPass::DefineAttachment(const AttachmentDescription& attachmentDescription)
    {
        m_AttachmentDescriptions.push_back(attachmentDescription);
        return m_AttachmentDescriptions.size() - 1;
    }


    void MetalRenderPass::AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_ColorAttachments.push_back(attachmentReference);
    }


    void MetalRenderPass::AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_ResolveAttachments.push_back(attachmentReference);
    }


    void MetalRenderPass::AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {
        AttachmentReference attachmentReference = {
            .AttachmentIndex = attachmentIndex,
            .OptimalImageLayout = optimalImageLayout
        };
        m_DepthStencilAttachment = attachmentReference;
    }


    void MetalRenderPass::EndBuildingRenderPass()
    {

    }


    void MetalRenderPass::UpdateAttachmentDefinition(AttachmentIndex attachmentIndex, const AttachmentDescription& attachmentDescription)
    {
        m_AttachmentDescriptions[attachmentIndex] = attachmentDescription;
    }


    void MetalRenderPass::Invalidate()
    {

    }


    AttachmentDescription MetalRenderPass::GetAttachmentDescription(AttachmentIndex attachmentIndex) const
    {
        ASSERT(attachmentIndex >= 0 && attachmentIndex < m_AttachmentDescriptions.size(), "The given attachment index does not fit the bounds of the attachment descriptions vector!");
        return m_AttachmentDescriptions[attachmentIndex];
    }


    uint32 MetalRenderPass::GetNumAttachments() const
    {
        return m_AttachmentDescriptions.size();
    }


    const std::vector<AttachmentReference>& MetalRenderPass::GetColorAttachmentReferences() const
    {
        return m_ColorAttachments;
    }


    const std::vector<AttachmentReference>& MetalRenderPass::GetResolveAttachmentReferences() const
    {
        return m_ResolveAttachments;
    }


    AttachmentReference MetalRenderPass::GetDepthStencilAttachmentReference() const
    {
        return m_DepthStencilAttachment;
    }

}
