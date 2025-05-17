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
        int32 WindowWidth;
        int32 WindowHeight;
        VkFormat Format;
    };

    class VulkanRenderPass : public RenderPass
    {
    public:
        explicit VulkanRenderPass(const VulkanRenderpassDesc& desc);
        ~VulkanRenderPass() override;

        void BeginRenderPass(CommandBufferHandle commandBufferHandle, FramebufferHandle frameBufferHandle) override;
        void EndRenderPass(CommandBufferHandle commandBufferHandle) override;

        void* GetNativeHandle() override { return m_Renderpass; }

    private:

        void CreateRenderPass();
        void DestroyRenderPass();

        VkDevice m_Device;
        uint32 m_WindowWidth;
        uint32 m_WindowHeight;
        VkFormat m_Format;

        VkRenderPass m_Renderpass;
    };

}
