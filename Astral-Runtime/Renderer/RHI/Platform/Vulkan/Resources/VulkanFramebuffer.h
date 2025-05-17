/**
* @file VulkanFramebuffer.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/Framebuffer.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanFramebufferDesc
    {
        VkDevice Device;
        VkRenderPass RenderPass;
        int32 WindowWidth;
        int32 WindowHeight;
        VkImageView ImageView;
    };

    class VulkanFramebuffer : public Framebuffer
    {
    public:
        explicit VulkanFramebuffer(const VulkanFramebufferDesc& desc);
        ~VulkanFramebuffer() override;

        void* GetNativeHandle() override { return m_Framebuffer; }

    private:

        void CreateFramebuffer();
        void DestroyFramebuffer();

        VkDevice m_Device;
        VkRenderPass m_RenderPass;
        uint32 m_WindowWidth;
        uint32 m_WindowHeight;
        VkImageView m_ImageView;

        VkFramebuffer m_Framebuffer;
    };

}
