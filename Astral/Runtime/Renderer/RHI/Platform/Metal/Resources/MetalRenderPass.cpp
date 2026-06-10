/**
* @file MetalRenderPass.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalRenderPass.h"

namespace Astral {

    MetalRenderPass::MetalRenderPass(const MetalRenderPassDesc& renderPassDesc)
    {

    }


    MetalRenderPass::~MetalRenderPass()
    {

    }


    void MetalRenderPass::BeginBuildingRenderPass()
    {

    }


    AttachmentIndex MetalRenderPass::DefineAttachment(const AttachmentDescription& attachmentDescription)
    {

    }


    void MetalRenderPass::AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {

    }


    void MetalRenderPass::AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {

    }


    void MetalRenderPass::AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
    {

    }


    void MetalRenderPass::EndBuildingRenderPass()
    {

    }


    void MetalRenderPass::Invalidate()
    {

    }


    AttachmentDescription MetalRenderPass::GetAttachmentDescription(AttachmentIndex attachmentIndex) const
    {

    }


    uint32 MetalRenderPass::GetNumAttachments() const
    {

    }


    const std::vector<AttachmentReference>& MetalRenderPass::GetColorAttachmentReferences() const
    {

    }


    const std::vector<AttachmentReference>& MetalRenderPass::GetResolveAttachmentReferences() const
    {

    }


    AttachmentReference MetalRenderPass::GetDepthStencilAttachmentReference() const
    {

    }

}
