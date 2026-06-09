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


    void MetalRenderPass::BeginBuildingSubpass()
    {

    }


    void MetalRenderPass::AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout)
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


    void MetalRenderPass::PreserveAttachment(AttachmentIndex attachmentIndex)
    {

    }


    SubpassIndex MetalRenderPass::EndBuildingSubpass()
    {

    }


    void MetalRenderPass::DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks)
    {

    }


    void MetalRenderPass::EndBuildingRenderPass()
    {

    }


    void MetalRenderPass::Invalidate()
    {

    }


    uint32 MetalRenderPass::GetNumberOfSubpasses()
    {

    }


    uint32 MetalRenderPass::GetNumColorAttachments(SubpassIndex subpassIndex)
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


    const std::vector<Vec4>& MetalRenderPass::GetClearColors() const
    {

    }


    void* MetalRenderPass::GetNativeHandle()
    {

    }


    void MetalRenderPass::CreateRenderPass()
    {

    }


    void MetalRenderPass::DestroyRenderPass()
    {

    }


    void MetalRenderPass::UpdateSubpassAttachmentLayouts()
    {

    }

}
