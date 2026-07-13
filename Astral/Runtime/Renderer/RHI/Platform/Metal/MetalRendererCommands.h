/**
* @file MetalRendererCommands.h
* @author Andrew Fagan
* @date 6/27/26
*/

#pragma once

#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    class MetalRendererCommands : public RendererCommands
    {
    public:

        void ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback) override;

        void NameObject(BufferHandle bufferHandle, std::string_view name) override;
        void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name) override;
        void NameObject(TextureHandle textureHandle, std::string_view name) override;
        void NameObject(ShaderHandle shaderHandle, std::string_view name) override;
        void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) override;

        void CallImGuiDraws(CommandBufferHandle commandBufferHandle) override;
    };

}