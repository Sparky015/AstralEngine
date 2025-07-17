/**
* @file VulkanFramebuffer.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/Framebuffer.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    struct VulkanFramebufferDesc
    {
        VkDevice Device;
        VkRenderPass RenderPass;
    };

    class VulkanFramebuffer : public Framebuffer
    {
    public:
        explicit VulkanFramebuffer(const VulkanFramebufferDesc& desc);
        ~VulkanFramebuffer() override;

        UVec2 GetExtent() override { return {m_FramebufferWidth, m_FramebufferHeight}; }

        void BeginBuildingFramebuffer(uint32 framebufferWidth, uint32 framebufferHeight) override;
        void AttachRenderTarget(RenderTargetHandle renderTargetHandle) override;
        void AttachTexture(TextureHandle textureHandle) override;
        void EndBuildingFramebuffer() override;

        void InvalidateFramebuffer() override;

        void* GetNativeHandle() override { return m_Framebuffer; }

    private:

        void CreateFramebuffer();
        void DestroyFramebuffer();

        VkDevice m_Device;
        VkRenderPass m_RenderPass;

        // Hold a ref to each image added so they will stay alive while framebuffer is alive
        std::vector<TextureHandle> m_Textures;
        std::vector<RenderTargetHandle> m_RenderTargets;
        std::vector<VkImageView> m_ImageViews;

        uint32 m_FramebufferWidth;
        uint32 m_FramebufferHeight;

        VkFramebuffer m_Framebuffer;
    };

}
