/**
* @file VulkanRenderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/Renderpass.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
    * @brief Information to create a Vulkan render pass from
    */
    struct VulkanRenderpassDesc
    {
        VkDevice Device;
    };

    /**
     * @brief A support wrapper around Vulkan RenderPass object
     * @warning The current implementation only supports single threaded recording
     */
    class VulkanRenderPass : public RenderPass
    {
    public:
        explicit VulkanRenderPass(const VulkanRenderpassDesc& desc);
        ~VulkanRenderPass() override;

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
         * @param attachmentIndex The attachment index of the attachment to add as a color attachment
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
         * @brief Updates the attachment definition of the attachment at the given index
         * @param attachmentIndex The index of the attachment to update the description of
         * @param attachmentDescription The new attachment description to update with
         */
        void UpdateAttachmentDefinition(AttachmentIndex attachmentIndex, const AttachmentDescription& attachmentDescription) override;

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
         * @brief Gets the color attachment descriptions of the render pass
         * @return The color attachment descriptions of the render pass
         */
        const std::vector<AttachmentReference>& GetColorAttachmentReferences() const override;

        /**
         * @brief Gets the color attachment descriptions of the render pass
         * @return The color attachment descriptions of the render pass
         */
        const std::vector<AttachmentReference>& GetResolveAttachmentReferences() const override;

        /**
         * @brief Gets the color attachment descriptions of the render pass
         * @return The color attachment descriptions of the render pass
         */
        AttachmentReference GetDepthStencilAttachmentReference() const override;

    private:

        std::vector<AttachmentDescription> m_AttachmentDescriptions;
        std::vector<AttachmentReference> m_ColorAttachments;
        std::vector<AttachmentReference> m_ResolveAttachments;
        AttachmentReference m_DepthStencilAttachment;
    };

}
