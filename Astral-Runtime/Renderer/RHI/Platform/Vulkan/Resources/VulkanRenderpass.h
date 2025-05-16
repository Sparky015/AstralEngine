/**
* @file VulkanRenderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/Renderpass.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanRenderpassDesc
    {

    };

    class VulkanRenderpass : public Renderpass
    {
    public:
        explicit VulkanRenderpass(const VulkanRenderpassDesc& desc);
        ~VulkanRenderpass() override;

        void BeginRenderpass(CommandBufferHandle commandBufferHandle) override;
        void EndRenderpass(CommandBufferHandle commandBufferHandle) override;

    private:

        void CreateRenderPass();
        void DestoryRenderpass();

        VkRenderPass m_Renderpass;
    };

}
