//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "../SceneRenderer.h"
#include "RendererCommands.h"
#include "Resources/Device.h"

namespace Astral {

    class RendererAPI
    {
    public:
        static Device& GetDevice();
        static void Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle);
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElementsIndexed(CommandBufferHandle commandBuffer, IndexBufferHandle indexBuffer);
        static void PushConstants(CommandBufferHandle commandBuffer, PipelineStateObjectHandle pipelineStateObjectHandle, void* data, uint32 sizeInBytes);
        static void SetBlending(bool enable);
        static void BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color);
        static void EndLabel(CommandBufferHandle commandBufferHandle);
        static void InsertMarker(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color);
        static void NameObject(BufferHandle bufferHandle, std::string_view name);
        static void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name);
        static void NameObject(TextureHandle textureHandle, std::string_view name);
        static void NameObject(ShaderHandle shaderHandle, std::string_view name);

        static void CallImGuiDraws(CommandBufferHandle commandBufferHandle);

    private:

        friend SceneRenderer;
        static RendererCommands* s_RendererCommands;
    };

}
