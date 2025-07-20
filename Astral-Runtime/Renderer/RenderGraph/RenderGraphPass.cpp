/**
* @file RenderGraphPass.cpp
* @author Andrew Fagan
* @date 7/7/2025
*/

#include "RenderGraphPass.h"

namespace Astral {

    RenderGraphPass::RenderGraphPass(Vec2 resourceDimensions, const std::string_view& name, const std::function<void()>& callback) :
        m_WriteAttachmentDimensions(resourceDimensions),
        m_Callback(callback),
        m_Name(name)
    {}

    bool RenderGraphPass::ExternalAttachment::operator==(const ExternalAttachment& other) const noexcept
    {
        return OwningPass == other.OwningPass && Name == other.Name && OptimalImageLayout == other.OptimalImageLayout;
    }


    void RenderGraphPass::LinkReadInputAttachment(RenderGraphPass* owningSubpass, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        ExternalAttachment externalAttachment = {
            .OwningPass = owningSubpass,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout,
        };

        m_ReadInputAttachments.push_back(externalAttachment);
    }


    void RenderGraphPass::LinkWriteInputAttachment(RenderGraphPass* owningSubpass, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        ExternalAttachment externalAttachment = {
            .OwningPass = owningSubpass,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout,
        };

        m_WriteInputAttachments.push_back(externalAttachment);
    }


    void RenderGraphPass::CreateColorAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout,
            .InitialLayout = attachmentDescription.InitialLayout,
            .AttachmentType = AttachmentType::COLOR,
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_ColorAttachments.push_back(attachmentIndex);
    }


    void RenderGraphPass::CreateResolveAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout,
            .InitialLayout = attachmentDescription.InitialLayout,
            .AttachmentType = AttachmentType::RESOLVE,
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_ResolveAttachments.push_back(attachmentIndex);
    }


    void RenderGraphPass::CreateDepthStencilAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout)
    {
        LocalAttachment localAttachment = {
            .AttachmentDescription = attachmentDescription,
            .Name = name,
            .OptimalImageLayout = optimalImageLayout,
            .InitialLayout = attachmentDescription.InitialLayout,
            .AttachmentType = AttachmentType::DEPTH_STENCIL,
        };

        m_Attachments.push_back(localAttachment);

        AttachmentIndex attachmentIndex = m_Attachments.size() - 1;
        m_DepthStencilAttachment = attachmentIndex;
    }


    AttachmentIndex RenderGraphPass::GetLocalAttachment(const std::string_view& name) const
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
                m_ReadInputAttachments == other.m_ReadInputAttachments &&
                m_ColorAttachments == other.m_ColorAttachments &&
                m_ResolveAttachments == other.m_ResolveAttachments &&
                m_DepthStencilAttachment == other.m_DepthStencilAttachment;
    }

}
