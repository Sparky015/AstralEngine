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
        void SetBlending(bool enable) override;

        void CallImGuiDraws(CommandBufferHandle commandBufferHandle) override;
    };

}
