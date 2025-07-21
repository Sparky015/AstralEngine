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
        void Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle) override;
        void SetClearColor(float r, float g, float b, float a) override;
        void DrawElementsIndexed(CommandBufferHandle commandBufferHandle, IndexBufferHandle indexBufferHandle) override;
        void PushConstants(CommandBufferHandle commandBufferHandle, PipelineStateObjectHandle
                                pipelineStateObjectHandle, void* data, uint32 sizeInBytes) override;
        void SetPipelineBarrier(CommandBufferHandle commandBufferHandle, const PipelineBarrier& pipelineBarrier) override;

        void SetBlending(bool enable) override;
        void BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color) override;
        void EndLabel(CommandBufferHandle commandBufferHandle) override;
        void InsertMarker(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color) override;
        void NameObject(BufferHandle bufferHandle, std::string_view name) override;
        void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name) override;
        void NameObject(TextureHandle textureHandle, std::string_view name) override;
        void NameObject(ShaderHandle shaderHandle, std::string_view name) override;
        void NameObject(FramebufferHandle framebufferHandle, std::string_view name) override;
        void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) override;

        void CallImGuiDraws(CommandBufferHandle commandBufferHandle) override;
    };

}
