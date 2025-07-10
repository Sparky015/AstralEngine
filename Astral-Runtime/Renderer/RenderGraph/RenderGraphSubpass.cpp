/**
* @file RenderGraphSubpass.cpp
* @author Andrew Fagan
* @date 7/7/2025
*/

#include "RenderGraphSubpass.h"

namespace Astral {

    bool RenderGraphSubpass::ExternalAttachment::operator==(const ExternalAttachment& other) const noexcept
    {
        return OwningSubpass == other.OwningSubpass && Name == other.Name && OptimalImageLayout == other.OptimalImageLayout;
    }


    void RenderGraphSubpass::AddInputAttachment(const RenderGraphSubpass& subpass, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        ExternalAttachment externalAttachment = {
            .OwningSubpass = subpass,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout
        };

        m_InputAttachments.push_back(externalAttachment);
    }


    void RenderGraphSubpass::AddColorAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
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


    void RenderGraphSubpass::AddResolveAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
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


    void RenderGraphSubpass::AddDepthStencilAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
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


    AttachmentIndex RenderGraphSubpass::GetAttachment(const std::string_view& name) const
    {
        for (int i = 0; i < m_Attachments.size(); i++)
        {
            if (m_Attachments[i].Name == name) { return i; }
        }

        return NullAttachmentIndex;
    }


    bool RenderGraphSubpass::operator==(const RenderGraphSubpass& other) const noexcept
    {
        return m_Attachments == other.m_Attachments &&
                m_InputAttachments == other.m_InputAttachments &&
                m_ColorAttachments == other.m_ColorAttachments &&
                m_ResolveAttachments == other.m_ResolveAttachments &&
                m_DepthStencilAttachment == other.m_DepthStencilAttachment;
    }

}
