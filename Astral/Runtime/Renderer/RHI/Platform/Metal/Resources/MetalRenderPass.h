/**
* @file MetalRenderPass.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/Renderpass.h"

#include "Metal/MTLDevice.hpp"
#include "Metal/MTLRenderPass.hpp"

namespace Astral {

    /**
    * @brief Information to create a Metal render pass from
    */
    struct MetalRenderPassDesc
    {
        MTL::Device* Device;
    };

    class MetalRenderPass : public RenderPass // TODO
    {
    public:

        MetalRenderPass(const MetalRenderPassDesc& renderPassDesc); // TODO
        ~MetalRenderPass() override; // TODO

        /**
         * @brief Indicates the start of defining a render pass
         */
        void BeginBuildingRenderPass() override;

        /**
         * @brief Defines an attachment for this render pass
         * @param attachmentDescription A description of an attachment
         * @return The attachment index that can be used to reference this attachment in the render pass
         */
        AttachmentIndex DefineAttachment(const AttachmentDescription& attachmentDescription) override;

        /**
         * @brief Adds a color attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as an color attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        void AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;

        /**
         * @brief Adds a resolve attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as a resolve attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        void AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;

        /**
         * @brief Adds a depth-stencil attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as a depth-stencil attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        void AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;

        /**
         * @brief Indicates the end of building the render pass
         * @pre @ref BeginBuildingRenderPass is called first to indicate the start of building the render pass
         */
        void EndBuildingRenderPass() override;

        /**
        * @brief Cleans up and resets an existing render pass if applicable
        */
        void Invalidate() override;

        /**
         * @brief Gets the attachment description of an attachment
         * @param attachmentIndex The attachment index of the attachment
         * @return The attachment description of an attachment
         */
        AttachmentDescription GetAttachmentDescription(AttachmentIndex attachmentIndex) const override;

        /**
         * @brief Gets the number of attachments in the render pass
         * @return The number of attachments in the render pass
         */
        uint32 GetNumAttachments() const override;

        /**
         * @brief Gets the color attachment references of the render pass
         * @return The color attachment references of the render pass
         */
        const std::vector<AttachmentReference>& GetColorAttachmentReferences() const override;

        /**
         * @brief Gets the resolve attachment references of the render pass
         * @return The resolve attachment references of the render pass
         */
        const std::vector<AttachmentReference>& GetResolveAttachmentReferences() const override;

        /**
        * @brief Gets the depth-stencil attachment reference of the render pass
        * @return The depth-stencil attachment reference of the render pass
        */
        AttachmentReference GetDepthStencilAttachmentReference() const override;

    private:


    };

}
