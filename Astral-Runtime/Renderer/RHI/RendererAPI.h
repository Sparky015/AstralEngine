//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "../SceneRenderer.h"
#include "RendererCommands.h"
#include "RendererContext.h"
#include "../Backends/ForwardRenderer.h"
#include "Resources/Device.h"

namespace Astral {

    class RendererAPI
    {
    public:
        static Device& GetDevice();
        static RenderingContext& GetContext();
        static void Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle);
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElementsIndexed(CommandBufferHandle commandBuffer, IndexBufferHandle indexBuffer);
        static void PushConstants(CommandBufferHandle commandBuffer, PipelineStateHandle pipelineStateObjectHandle, void* data, uint32 sizeInBytes);
        static void SetPipelineBarrier(CommandBufferHandle commandBufferHandle, const PipelineBarrier& pipelineBarrier);
        static void SetBlending(bool enable); // TODO: Remove this
        static void BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color);
        static void EndLabel(CommandBufferHandle commandBufferHandle);
        static void InsertMarker(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color);
        static void NameObject(BufferHandle bufferHandle, std::string_view name);
        static void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name);
        static void NameObject(TextureHandle textureHandle, std::string_view name);
        static void NameObject(ShaderHandle shaderHandle, std::string_view name);
        static void NameObject(FramebufferHandle framebufferHandle, std::string_view name);
        static void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name);
        static void NameObject(RenderPassHandle renderPassHandle, std::string_view name);


        static void CallImGuiDraws(CommandBufferHandle commandBufferHandle);

    private:

        friend SceneRenderer;
        static RendererCommands* s_RendererCommands;
    };

}
