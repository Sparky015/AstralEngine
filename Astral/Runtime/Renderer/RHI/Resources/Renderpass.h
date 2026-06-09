/**
* @file Renderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Framebuffer.h"
#include "Renderer/RHI/Common/AccessFlags.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/PipelineStageFlags.h"
#include "Renderer/RHI/Common/SampleCount.h"


namespace Astral {

    using AttachmentIndex = uint32;
    static constexpr AttachmentIndex NullAttachmentIndex = -1;
    using SubpassIndex = uint8;
    static constexpr SubpassIndex NullSubpassIndex = -1;
    static constexpr SubpassIndex SubpassExternal = -1;

    /**
     * @brief Defines a load operation for a render pass attachment
     */
    enum class AttachmentLoadOp : uint8
    {
        LOAD,
        CLEAR,
        DONT_CARE,
        NONE
    };

    /**
     * @brief Defines a store operation for a render pass attachment
     */
    enum class AttachmentStoreOp : uint8
    {
        STORE,
        DONT_CARE,
        NONE
    };

    /**
     * @brief Defines an attachment and how to use the attachment in a render pass
     */
    struct AttachmentDescription
    {
        ImageFormat Format;
        ImageUsageFlags ImageUsageFlags;
        AttachmentLoadOp LoadOp;
        AttachmentStoreOp StoreOp;
        ImageLayout InitialLayout;
        ImageLayout FinalLayout;
        Vec4 ClearColor; // Only use when load op is set to clear
        SampleCount MSAASamples = SampleCount::SAMPLE_1_BIT;
        uint32 LayerCount = 1;
        uint32 MipMapCount = 1;
        TextureType TextureType = TextureType::IMAGE_2D;

        bool operator==(const AttachmentDescription&) const = default;
    };

    /**
     * @brief Holds a reference index to an attachment as well as the layout to use for the attachment in a render pass
     */
    struct AttachmentReference
    {
        AttachmentIndex AttachmentIndex = NullAttachmentIndex;
        ImageLayout OptimalImageLayout = ImageLayout::UNDEFINED;
    };

    static constexpr uint32 FullSubresourceRange = -1;

    /**
     * @brief Holds a resource to back an attachment with fields to specify the subresource range of the resource to use
     */
    struct AttachmentResource
    {
        TextureHandle Resource;
        uint32 MipLevel; /// Specify a specific mip map level or use FullSubresourceRange var to use all mip map levels in texture
        uint32 ArrayLayer; /// Specify a specific layer number or use FullSubresourceRange var to use all layers in texture
    };

    /**
     * @brief Defines information needed to declare a dependency between two subpasses
     */
    struct SubpassDependencyMasks
    {
        PipelineStageFlags SourceStageMask;
        PipelineStageFlags DestinationStageMask;
        AccessFlags SourceAccessMask;
        AccessFlags DestinationAccessMask;
    };

    /**
     * @brief Defines the RHI RenderPass object
     * @warning The current implementation only supports single threaded recording
     */
    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        /**
         * @brief Indicates the start of defining a render pass
         */
        virtual void BeginBuildingRenderPass() = 0;

        /**
         * @brief Defines an attachment for this render pass
         * @param attachmentDescription A description of an attachment
         * @return The attachment index that can be used to reference this attachment in the render pass
         */
        virtual AttachmentIndex DefineAttachment(const AttachmentDescription& attachmentDescription) = 0;

        /**
         * @brief Indicates the start of defining a render pass subpass
         */
        virtual void BeginBuildingSubpass() = 0;

        /**
         * @brief Adds an input attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as an input attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        virtual void AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;

        /**
         * @brief Adds a color attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as a color attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        virtual void AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;

        /**
         * @brief Adds a resolve attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as a resolve attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        virtual void AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;

        /**
         * @brief Adds a depth-stencil attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as a depth-stencil attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        virtual void AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;

        /**
         * @brief Instructs the render pass to preserve an attachment if the attachment does not get read or written to for this subpass
         * @param attachmentIndex The attachment index of the attachment to preserve
         */
        virtual void PreserveAttachment(AttachmentIndex attachmentIndex) = 0;

        /**
         * @brief Indicates the end of building the current subpass
         * @pre @ref BeginBuildingSubpass is called first to indicate the start of building the current subpass
         */
        virtual SubpassIndex EndBuildingSubpass() = 0;

        /**
         * @brief Defines a subpass dependency between two subpasses
         * @param sourceSubpass The source subpass
         * @param destinationSubpass The destination subpass that depends on the source subpass
         * @param subpassDependencyMasks Pipeline and access masks of the subpass dependency
         */
        virtual void DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks) = 0;

        /**
         * @brief Indicates the end of building the render pass
         * @pre @ref BeginBuildingRenderPass is called first to indicate the start of building the render pass
         */
        virtual void EndBuildingRenderPass() = 0;

        /**
        * @brief Cleans up and resets an existing render pass if applicable
        */
        virtual void Invalidate() = 0;

        /**
         * @brief Resets framebuffer texture layout metadata
         * @param frameBufferHandle The framebuffer being used with the render pass
         */
        virtual void BeginRenderPass(FramebufferHandle frameBufferHandle) = 0;

        /**
        * @brief Updates attachment texture layout metadata
        */
        virtual void NextSubpass() = 0;

        /**
        * @brief Updates attachment texture layout metadata to final layouts
        */
        virtual void EndRenderPass() = 0;

        /**
        * @brief Gets the number of subpasses in the render pass
        * @return The number of subpasses in the render pass
        */
        virtual uint32 GetNumberOfSubpasses() = 0;

        /**
         * @brief Gets the attachment description of an attachment
         * @param attachmentIndex The attachment index of the attachment
         * @return The attachment description of an attachment
         */
        virtual AttachmentDescription GetAttachmentDescription(AttachmentIndex attachmentIndex) const = 0;

        /**
         * @brief Gets the number of attachments in the render pass
         * @return The number of attachments in the render pass
         */
        virtual uint32 GetNumAttachments() const = 0;

        /**
         * @brief Gets the color attachment references of the render pass
         * @return The color attachment references of the render pass
         */
        virtual const std::vector<AttachmentReference>& GetColorAttachmentReferences() const = 0;

        /**
         * @brief Gets the resolve attachment references of the render pass
         * @return The resolve attachment references of the render pass
         */
        virtual const std::vector<AttachmentReference>& GetResolveAttachmentReferences() const = 0;

        /**
         * @brief Gets the depth-stencil attachment reference of the render pass
         * @return The depth-stencil attachment reference of the render pass
         */
        virtual AttachmentReference GetDepthStencilAttachmentReference() const = 0;

        /**
        * @brief Gets the number of color attachments in a subpass
        * @param subpassIndex The subpass to get the number of color attachments from
        * @return The number of color attachments in a subpass
        */
        virtual uint32 GetNumColorAttachments(SubpassIndex subpassIndex) = 0;

        /**
         * @brief Gets the clear colors of every attachment (that has a clear load op)
         * @return The clear colors of every attachment (that has a clear load op)
         */
        virtual const std::vector<Vec4>& GetClearColors() const = 0;

        /**
         * @brief Gets the native render pass object of the current RHI backend
         * @return The native render pass object of the current RHI backend
         */
        virtual void* GetNativeHandle() = 0;
    };

    using RenderPassHandle = GraphicsRef<RenderPass>;

}
