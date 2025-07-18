/**
* @file VulkanRenderTarget.h
* @author Andrew Fagan
* @date 5/15/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/RenderTarget.h"

#include <vulkan/vulkan_core.h>

#include "VulkanTexture.h"

namespace Astral {

    struct VulkanRenderTargetDesc
    {
        VkDevice Device;
        VkImage Image;
        VkImageView ImageView;
        ImageFormat Format;
        ImageLayout Layout;
        uint32 ImageIndex;
        VkSemaphore ImageAvailableSemaphore;
        VkSemaphore RenderCompleteSemaphore;
        uint32 ImageWidth;
        uint32 ImageHeight;
    };

    class VulkanRenderTarget : public RenderTarget
    {
    public:
        explicit VulkanRenderTarget(const VulkanRenderTargetDesc& desc);
        ~VulkanRenderTarget() override;

        ImageFormat GetImageFormat() override { return m_ImageFormat; }
        UVec2 GetDimensions() override { return UVec2(m_ImageWidth, m_ImageHeight); }

        uint32 GetImageIndex() override { return m_ImageIndex; }

        void* GetImageAvailableSemaphore() override { return m_ImageAvailableSemaphore; }
        void* GetRenderCompleteSemaphore() override { return m_RenderCompleteSemaphore; }
        void* GetFence() override { return m_Fence; }
        void* GetImageView() override { return m_ImageView; }
        void* GetNativeHandle() override { return m_Image; }

        TextureHandle GetAsTexture() override { return m_RenderTargetTexture; }

        void SetSyncPrimatives(void* renderCompleteSemaphore, void* imageAvailableSemaphore, void* fence) override
        {
            m_ImageAvailableSemaphore = (VkSemaphore)imageAvailableSemaphore;
            m_RenderCompleteSemaphore = (VkSemaphore)renderCompleteSemaphore;
            m_Fence = (VkFence)fence;
        }

    private:


        VkImage m_Image;
        VkImageView m_ImageView;
        ImageFormat m_ImageFormat;
        ImageLayout m_ImageLayout;

        TextureHandle m_RenderTargetTexture;

        uint32 m_ImageIndex;
        VkSemaphore m_RenderCompleteSemaphore;
        VkSemaphore m_ImageAvailableSemaphore;
        VkFence m_Fence;

        uint32 m_ImageWidth;
        uint32 m_ImageHeight;
    };

}
