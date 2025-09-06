/**
* @file VulkanRendererCommands.h
* @author Andrew Fagan
* @date 3/4/25
*/

#pragma once

#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    class VulkanRendererCommands : public RendererCommands
    {
    public:

        void ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback) override;

        void NameObject(BufferHandle bufferHandle, std::string_view name) override;
        void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name) override;
        void NameObject(TextureHandle textureHandle, std::string_view name) override;
        void NameObject(ShaderHandle shaderHandle, std::string_view name) override;
        void NameObject(FramebufferHandle framebufferHandle, std::string_view name) override;
        void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) override;
        void NameObject(RenderPassHandle renderPassHandle, std::string_view name) override;

        void CallImGuiDraws(CommandBufferHandle commandBufferHandle) override;
    };

}
