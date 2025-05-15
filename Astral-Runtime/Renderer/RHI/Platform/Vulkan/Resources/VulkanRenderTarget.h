/**
* @file VulkanRenderTarget.h
* @author Andrew Fagan
* @date 5/15/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/RenderTarget.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanRenderTargetDesc
    {
        VkImage Image;
        VkImageView ImageView;
        uint32 ImageIndex;
    };

    class VulkanRenderTarget : public RenderTarget
    {
    public:
        VulkanRenderTarget(const VulkanRenderTargetDesc& desc);
        ~VulkanRenderTarget() override;

        uint32 GetImageIndex() override { return m_ImageIndex; }

        void* GetNativeHandle() override { return m_Image; }

    private:

        VkImage m_Image;
        VkImageView m_ImageView;
        uint32 m_ImageIndex;
    };

}
