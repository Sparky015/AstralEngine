/**
* @file RenderGraphPass.cpp
* @author Andrew Fagan
* @date 7/7/2025
*/

#include "RenderGraphPass.h"

namespace Astral {

    RenderGraphPass::RenderGraphPass(UVec2 resourceDimensions, const std::string_view& debugName, const std::function<void()>& callback) :
        m_ResourceDimensions(resourceDimensions),
        m_Callback(callback),
        m_DebugName(debugName)
    {}

    bool RenderGraphPass::ExternalAttachment::operator==(const ExternalAttachment& other) const noexcept
    {
        return OwningPass == other.OwningPass && Name == other.Name && OptimalImageLayout == other.OptimalImageLayout;
    }


    void RenderGraphPass::AddInputAttachment(const RenderGraphPass& subpass, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        ExternalAttachment externalAttachment = {
            .OwningPass = subpass,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout
        };

        m_InputAttachments.push_back(externalAttachment);
    }


    void RenderGraphPass::AddColorAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_ColorAttachments.push_back(attachmentIndex);
    }


    void RenderGraphPass::AddResolveAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_ResolveAttachments.push_back(attachmentIndex);
    }


    void RenderGraphPass::AddDepthStencilAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_DepthStencilAttachment = attachmentIndex;
    }


    AttachmentIndex RenderGraphPass::GetAttachment(const std::string_view& name) const
    {
        for (int i = 0; i < m_Attachments.size(); i++)
        {
            if (m_Attachments[i].Name == name) { return i; }
        }

        return NullAttachmentIndex;
    }


    bool RenderGraphPass::operator==(const RenderGraphPass& other) const noexcept
    {
        return m_Attachments == other.m_Attachments &&
                m_InputAttachments == other.m_InputAttachments &&
                m_ColorAttachments == other.m_ColorAttachments &&
                m_ResolveAttachments == other.m_ResolveAttachments &&
                m_DepthStencilAttachment == other.m_DepthStencilAttachment;
    }

}
