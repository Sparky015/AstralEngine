/**
* @file RenderGraphPass.h
* @author Andrew Fagan
* @date 7/7/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Renderpass.h"

#include <functional>

namespace Astral {

    // Used to reference the dimensions of the output texture/render target of the render graph
    static constexpr Vec2 OutputAttachmentDimensions = Vec2(-1);

    enum class AttachmentType
    {
        COLOR,
        DEPTH_STENCIL,
        RESOLVE
    };

    /**
     * @brief Defines a render pass that can be added to a render graph
     * @warning Each instance should only be used with one render graph at a time
     */
    class RenderGraphPass
    {
    public:

        /**
         * @brief Describes an attachment that is created by this render pass
         * @warning The AttachmentDescription member is modified during render graph creation. Do not rely on the image layouts, load ops, and store ops staying the same.
         */
        struct LocalAttachment
        {
            AttachmentDescription AttachmentDescription;
            std::string_view Name;
            ImageLayout OptimalImageLayout;
            ImageLayout InitialLayout;
            AttachmentType AttachmentType;
            ImageLayout LastKnownLayout;

            bool operator==(const LocalAttachment& other) const noexcept = default;
        };

        /**
         * @brief Describes an attachment that is used as an input by this render pass from another render pass
         */
        struct ExternalAttachment
        {
            RenderGraphPass* OwningPass;
            std::string_view Name;
            ImageLayout OptimalImageLayout;

            bool operator==(const ExternalAttachment& other) const noexcept;
        };

        /**
         * @brief Constructor for RenderGraphPass
         * @param resourceDimensions The resource dimensions for the textures being used as write attachments in the render pass
         * @param name The name for the render graph pass (should be unique in render graph)
         * @param callback Callback that will actually execute on the render pass and make the API calls
         */
        explicit RenderGraphPass(Vec2 resourceDimensions, const std::string_view& name, const std::function<void()>& callback);

        /**
         * @brief Links an attachment from another render pass to use as read input in this render pass
         * @param owningSubpass Subpass that owns the attachment
         * @param name Name of the attachment from owningSubpass
         * @param optimalImageLayout The optimal image layout to put the attachment in when this render pass is executed
         * @post  The owningSubpass pointer should remain valid until the render graph is built (@ref RenderGraph::EndBuildingRenderGraph is called)
         */
        void LinkReadInputAttachment(RenderGraphPass* owningSubpass, const std::string_view& name, ImageLayout optimalImageLayout);

        /**
         * @brief Links an attachment from another render pass to use as input in this render pass
         * @param owningSubpass Subpass that owns the attachment
         * @param name Name of the attachment from owningSubpass
         * @param optimalImageLayout The optimal image layout to put the attachment in when this render pass is executed
         * @post  The owningSubpass pointer should remain valid until the render graph is built (@ref RenderGraph::EndBuildingRenderGraph is called)
         */
        void LinkWriteInputAttachment(RenderGraphPass* owningSubpass, const std::string_view& name, ImageLayout optimalImageLayout);

        /**
         * @brief Creates a color attachment and adds it to the render pass
         * @param attachmentDescription The description of the attachment to be created
         * @param name Name of the attachment
         * @param optimalImageLayout The optimal image layout to put the attachment in when this render pass is executed
         */
        void CreateColorAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);

        /**
         * @brief Creates a resolve attachment and adds it to the render pass
         * @param attachmentDescription The description of the attachment to be created
         * @param name Name of the attachment
         * @param optimalImageLayout The optimal image layout to put the attachment in when this render pass is executed
         */
        void CreateResolveAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);

        /**
         * @brief Creates a depth stencil attachment and adds it to the render pass
         * @param attachmentDescription The description of the attachment to be created
         * @param name Name of the attachment
         * @param optimalImageLayout The optimal image layout to put the attachment in when this render pass is executed
         */
        void CreateDepthStencilAttachment(const AttachmentDescription& attachmentDescription, const std::string_view& name, ImageLayout optimalImageLayout);

        /**
         * @brief Adds a dependency to this render pass
         * @param renderPass The render pass that the instance is dependent on
         */
        void AddDependency(RenderGraphPass* renderPass);

        /**
         * @brief Gets the attachments created by this render pass
         * @return The attachments created by this render pass
         * @note This is left non-const so attachment descriptions can be modified by the render graph for automatic barrier transition
         */
        std::vector<LocalAttachment>& GetAttachments() { return m_Attachments; }

        /**
         * @brief Gets the attachments created by this render pass
         * @return The attachments created by this render pass
         */
        const std::vector<LocalAttachment>& GetAttachments() const { return m_Attachments; }

        /**
         * @brief Gets the color attachment indices that were created by this render pass
         * @return The color attachment indices
         */
        const std::vector<AttachmentIndex>& GetColorAttachments() const { return m_ColorAttachments; }

        /**
         * @brief Gets the read input attachments that were linked to this render pass
         * @return The read input attachments linked to this render pass
         * @note This is left non-const so it can be modified by the render graph for memory safety reasons
         */
        std::vector<ExternalAttachment>& GetReadInputAttachments() { return m_ReadInputAttachments; }

        /**
         * @brief Gets the read input attachments that were linked to this render pass
         * @return The read input attachments linked to this render pass
         */
        const std::vector<ExternalAttachment>& GetReadInputAttachments() const { return m_ReadInputAttachments; }

        /**
         * @brief Gets the write input attachments that were linked to this render pass
         * @return The write input attachments linked to this render pass
         * @note This is left non-const so it can be modified by the render graph for memory safety reasons
         */
        std::vector<ExternalAttachment>& GetWriteInputAttachments() { return m_WriteInputAttachments; }

        /**
         * @brief Gets the write input attachments that were linked to this render pass
         * @return The write input attachments linked to this render pass
         */
        const std::vector<ExternalAttachment>& GetWriteInputAttachments() const { return m_WriteInputAttachments; }

        /**
         * @brief Gets the resolve attachment indices that were created by this render pass
         * @return The resolve attachment indices
         */
        const std::vector<AttachmentIndex>& GetResolveAttachments() const { return m_ResolveAttachments; }

        /**
         * @brief Gets the depth-stencil attachment index that was created by this render pass
         * @return The depth-stencil attachment index
         */
        AttachmentIndex GetDepthAttachment() const { return m_DepthStencilAttachment; }

        /**
         * @brief Gets an attachment created by this render pass by name
         * @return The attachment index of the attachment
         */
        AttachmentIndex GetLocalAttachment(const std::string_view& name) const;

        /**
         * @brief Gets the explicit dependencies to this render pass
         * @return The explicit dependencies to this render pass
         */
        std::vector<RenderGraphPass*> GetExplicitDependencies() const { return m_ExplicitDependencies; }

        /**
         * @brief Gets the explicit dependencies to this render pass
         * @return The explicit dependencies to this render pass
         * @note This is left non-const so it can be modified by the render graph for memory safety reasons
         */
        std::vector<RenderGraphPass*> GetExplicitDependencies() { return m_ExplicitDependencies; }

        /**
         * @brief Executes the render pass' rendering code
         */
        void Execute() const { m_Callback(); }

        /**
         * @brief Gets the resource dimensions of attachments being written to by the render pass
         */
        Vec2 GetWriteAttachmentDimensions() const { return m_WriteAttachmentDimensions; }

        /**
         * @brief Gets the name of this render pass
         */
        const std::string_view& GetName() const { return m_Name; }

        /**
         * @brief Equality operator for render passes
         */
        bool operator==(const RenderGraphPass& other) const noexcept;

    private:

        // All attachments created by this render pass
        std::vector<LocalAttachment> m_Attachments;
        std::vector<RenderGraphPass*> m_ExplicitDependencies;
        Vec2 m_WriteAttachmentDimensions;

        // All attachments linked to this render pass
        std::vector<ExternalAttachment> m_ReadInputAttachments;
        std::vector<ExternalAttachment> m_WriteInputAttachments;

        // Indices to m_Attachments that organize attachments into their attachment types
        std::vector<AttachmentIndex> m_ColorAttachments;
        std::vector<AttachmentIndex> m_ResolveAttachments;
        AttachmentIndex m_DepthStencilAttachment{NullAttachmentIndex};

        // Name of render pass that the render pass can be referenced by
        std::string_view m_Name;

        // The callback that will execute the rendering code for the render pass
        std::function<void()> m_Callback;
    };

}
