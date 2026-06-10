/**
* @file Renderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Renderer/RHI/Common/AccessFlags.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/PipelineStageFlags.h"
#include "Renderer/RHI/Common/SampleCount.h"
#include "Renderer/RHI/Resources/Texture.h"


namespace Astral {

    using AttachmentIndex = uint32;
    static constexpr AttachmentIndex NullAttachmentIndex = -1;

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
         * @brief Indicates the end of building the render pass
         * @pre @ref BeginBuildingRenderPass is called first to indicate the start of building the render pass
         */
        virtual void EndBuildingRenderPass() = 0;

        /**
        * @brief Cleans up and resets an existing render pass if applicable
        */
        virtual void Invalidate() = 0;

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

    };

    using RenderPassHandle = GraphicsRef<RenderPass>;

}
