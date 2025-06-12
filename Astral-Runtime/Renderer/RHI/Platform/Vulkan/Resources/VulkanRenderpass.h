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
        VkFormat Format;
    };

    class VulkanRenderPass : public RenderPass
    {
    public:
        explicit VulkanRenderPass(const VulkanRenderpassDesc& desc);
        ~VulkanRenderPass() override;

        void BeginBuildingRenderPass();
        void DefineAttachment();
        void BeginBuildingSubpass();
        void EndBuildingSubpass();
        void EndBuildingRenderPass();

        void BeginRenderPass(CommandBufferHandle commandBufferHandle, FramebufferHandle frameBufferHandle) override;
        void NextSubpass(CommandBufferHandle commandBufferHandle);
        void EndRenderPass(CommandBufferHandle commandBufferHandle) override;

        void* GetNativeHandle() override { return m_RenderPass; }

    private:

        void CreateRenderPass();
        void DestroyRenderPass();

        using AttachmentIndex = uint8;

        struct Subpass
        {
            VkSubpassDescription m_SubpassDescription;
            std::vector<AttachmentIndex> m_AttachmentIndices;
        };

        VkDevice m_Device;
        VkFormat m_Format;

        std::vector<VkAttachmentDescription> m_Attachments;
        std::vector<Subpass> m_Subpasses;

        VkRenderPass m_RenderPass;
    };

}
