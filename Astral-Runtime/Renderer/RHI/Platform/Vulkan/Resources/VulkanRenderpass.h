/**
* @file VulkanRenderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/Renderpass.h"
#include "Renderer/RHI/Resources/Framebuffer.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanRenderpassDesc
    {
        VkDevice Device;
    };

    class VulkanRenderPass : public RenderPass
    {
    public:
        explicit VulkanRenderPass(const VulkanRenderpassDesc& desc);
        ~VulkanRenderPass() override;

        void BeginBuildingRenderPass() override;

        AttachmentIndex DefineAttachment(const AttachmentDescription& attachmentDescription) override;
        void BeginBuildingSubpass() override;
        void AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;
        void AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;
        void AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;
        void AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) override;
        void PreserveAttachment(AttachmentIndex attachmentIndex) override;
        SubpassIndex EndBuildingSubpass() override;
        void DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks) override;
        void EndBuildingRenderPass() override;

        void Invalidate() override;

        void BeginRenderPass(CommandBufferHandle commandBufferHandle, FramebufferHandle frameBufferHandle) override;
        void NextSubpass(CommandBufferHandle commandBufferHandle) override;
        void EndRenderPass(CommandBufferHandle commandBufferHandle) override;

        uint32 GetNumberOfSubpasses() override { return (uint32)m_SubpassDescriptions.size(); }
        uint32 GetNumColorAttachments(SubpassIndex subpassIndex) override;

        void* GetNativeHandle() override { return m_RenderPass; }

    private:

        void CreateRenderPass();
        void DestroyRenderPass();

        VkAttachmentLoadOp ConvertToVkLoadOp(AttachmentLoadOp loadOp);
        VkAttachmentStoreOp ConvertToVkStoreOp(AttachmentStoreOp storeOp);

        struct SubpassAttachments
        {
            std::vector<VkAttachmentReference> InputAttachments;
            std::vector<VkAttachmentReference> ColorAttachments;
            std::vector<VkAttachmentReference> ResolveAttachments;
            std::vector<VkAttachmentReference> DepthStencilAttachment;
            std::vector<AttachmentIndex> PreserveAttachments;
        };

        VkDevice m_Device;

        std::vector<VkAttachmentDescription> m_RenderPassAttachments;
        std::vector<SubpassAttachments> m_SubpassAttachments;
        std::vector<VkSubpassDescription> m_SubpassDescriptions;
        std::vector<VkSubpassDependency> m_SubpassDependencies;
        std::vector<VkClearValue> m_ClearValues;

        VkRenderPass m_RenderPass;
    };

}
