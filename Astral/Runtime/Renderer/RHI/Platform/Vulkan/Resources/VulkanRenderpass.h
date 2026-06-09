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
         * @brief Indicates the start of defining a render pass subpass
         */
        void BeginBuildingSubpass() override;

        /**
         * @brief Adds an input attachment to the current subpass
         * @param attachmentIndex The attachment index of the attachment to add as an input attachment
         * @param optimalImageLayout The image layout to use for the attachment during the render pass
         * @pre @ref BeginBuildingRenderPass and @ref BeginBuildingSubpass should be called first to indicate that the render pass
         *           is being built as well as which subpass to add this attachment too
         */
        void AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;

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
         * @brief Instructs the render pass to preserve an attachment if the attachment does not get read or written to for this subpass
         * @param attachmentIndex The attachment index of the attachment to preserve
         */
        void PreserveAttachment(AttachmentIndex attachmentIndex) override;

        /**
         * @brief Indicates the end of building the current subpass
         * @pre @ref BeginBuildingSubpass is called first to indicate the start of building the current subpass
         */
        SubpassIndex EndBuildingSubpass() override;

        /**
         * @brief Defines a subpass dependency between two subpasses
         * @param sourceSubpass The source subpass
         * @param destinationSubpass The destination subpass that depends on the source subpass
         * @param subpassDependencyMasks Pipeline and access masks of the subpass dependency
         */
        void DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks) override;

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
        * @brief Gets the number of subpasses in the render pass
        * @return The number of subpasses in the render pass
        */
        uint32 GetNumberOfSubpasses() override { return (uint32)m_SubpassDescriptions.size(); }

        /**
        * @brief Gets the number of color attachments in a subpass
        * @param subpassIndex The subpass to get the number of color attachments from
        * @return The number of color attachments in a subpass
        */
        uint32 GetNumColorAttachments(SubpassIndex subpassIndex) override;

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

        /**
         * @brief Gets the clear colors of every attachment (that has a clear load op)
         * @return The clear colors of every attachment (that has a clear load op)
         */
        const std::vector<Vec4>& GetClearColors() const override { return m_ClearValues; }

        /**
         * @brief Gets the native render pass object of the current RHI backend (VkRenderPass)
         * @return The native render pass object of the current RHI backend (VkRenderPass)
         */
        void* GetNativeHandle() override { return m_RenderPass; }

    private:

        /**
         * @brief Creates the render pass object based on the provided render pass definitions
         */
        void CreateRenderPass();

        /**
         * @brief Destroys the render pass object
         */
        void DestroyRenderPass();

        /**
         * @brief Updates texture image layout metadata to the current subpass' optimal layouts
         */
        void UpdateSubpassAttachmentLayouts();

        struct SubpassAttachments
        {
            std::vector<VkAttachmentReference> InputAttachments;
            std::vector<VkAttachmentReference> ColorAttachments;
            std::vector<VkAttachmentReference> ResolveAttachments;
            std::vector<VkAttachmentReference> DepthStencilAttachment;
            std::vector<AttachmentIndex> PreserveAttachments;
        };

        VkDevice m_Device;

        std::vector<AttachmentDescription> m_AttachmentDescriptions;
        std::vector<AttachmentReference> m_ColorAttachments;
        std::vector<AttachmentReference> m_ResolveAttachments;
        AttachmentReference m_DepthStencilAttachment;

        std::vector<VkAttachmentDescription> m_RenderPassAttachments;
        std::vector<SubpassAttachments> m_SubpassAttachments;
        std::vector<VkSubpassDescription> m_SubpassDescriptions;
        std::vector<VkSubpassDependency> m_SubpassDependencies;
        std::vector<Vec4> m_ClearValues;

        VkRenderPass m_RenderPass;

        uint32 m_CurrentSubpassIndex{0}; // Updated each time a new subpass is started and reset when a render pass is started
    };

}
