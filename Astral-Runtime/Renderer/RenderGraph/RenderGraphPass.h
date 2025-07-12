/**
* @file RenderGraphSubpass.h
* @author Andrew Fagan
* @date 7/7/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Renderpass.h"

#include <functional>

namespace Astral {

    class RenderGraphPass
    {
    public:
        explicit RenderGraphPass(UVec2 resourceDimensions, const std::string_view& debugName, const std::function<void()>& callback);

        struct LocalAttachment
        {
            AttachmentDescription AttachmentDescription;
            std::string_view Name;
            ImageLayout OptimalImageLayout;

            bool operator==(const LocalAttachment& other) const noexcept = default;
        };

        struct ExternalAttachment
        {
            const RenderGraphPass& OwningPass;
            std::string_view Name;
            ImageLayout OptimalImageLayout;

            bool operator==(const ExternalAttachment& other) const noexcept;
        };

        void AddInputAttachment(const RenderGraphPass& subpass, const std::string_view& name, ImageLayout optimalImageLayout);
        void AddColorAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);
        void AddResolveAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);
        void AddDepthStencilAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);

        const std::vector<LocalAttachment>& GetAttachments() const { return m_Attachments; }
        const std::vector<AttachmentIndex>& GetColorAttachments() const { return m_ColorAttachments; }
        const std::vector<ExternalAttachment>& GetInputAttachments() const { return m_InputAttachments; }
        const std::vector<AttachmentIndex>& GetResolveAttachments() const { return m_ResolveAttachments; }
        AttachmentIndex GetDepthAttachment() const { return m_DepthStencilAttachment; }

        AttachmentIndex GetAttachment(const std::string_view& name) const;

        void Execute() const { m_Callback(); }

        UVec2 GetResourceDimensions() const { return m_ResourceDimensions; }
        const std::string_view& GetDebugName() const { return m_DebugName; }

        bool operator==(const RenderGraphPass& other) const noexcept;

    private:

        std::vector<LocalAttachment> m_Attachments;

        std::vector<ExternalAttachment> m_InputAttachments;
        std::vector<AttachmentIndex> m_ColorAttachments;
        std::vector<AttachmentIndex> m_ResolveAttachments;
        AttachmentIndex m_DepthStencilAttachment{NullAttachmentIndex};

        UVec2 m_ResourceDimensions;
        std::function<void()> m_Callback;

        std::string_view m_DebugName;
    };

}
