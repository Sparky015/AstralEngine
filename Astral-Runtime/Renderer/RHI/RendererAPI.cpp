//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Core/Engine.h"
#include "Platform/Vulkan/VulkanRendererCommands.h"
#include "Renderer/RendererManager.h"


namespace Astral {

    RendererCommands* RendererAPI::s_RendererCommands = new VulkanRendererCommands();


    Device& RendererAPI::GetDevice()
    {
        return Engine::Get().GetRendererManager().GetContext().GetDevice();
    }


    RenderingContext& RendererAPI::GetContext()
    {
        return Engine::Get().GetRendererManager().GetContext();
    }


    void RendererAPI::Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle)
    {
        s_RendererCommands->Clear(commandBufferHandle, renderTargetHandle);
    }


    void RendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        s_RendererCommands->SetClearColor(r, g, b, a);
    }


    void RendererAPI::DrawElementsIndexed(CommandBufferHandle commandBuffer, IndexBufferHandle indexBuffer)
    {
        s_RendererCommands->DrawElementsIndexed(commandBuffer, indexBuffer);
    }


    void RendererAPI::PushConstants(CommandBufferHandle commandBuffer, PipelineStateHandle pipelineStateObjectHandle, void* data, uint32 sizeInBytes)
    {
        s_RendererCommands->PushConstants(commandBuffer, pipelineStateObjectHandle, data, sizeInBytes);
    }


    void RendererAPI::SetPipelineBarrier(CommandBufferHandle commandBufferHandle, const PipelineBarrier& pipelineBarrier)
    {
        s_RendererCommands->SetPipelineBarrier(commandBufferHandle, pipelineBarrier);
    }


    void RendererAPI::SetBlending(bool enable)
    {
        s_RendererCommands->SetBlending(enable);
    }


    void RendererAPI::BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color)
    {
        s_RendererCommands->BeginLabel(commandBufferHandle, label, color);
    }


    void RendererAPI::EndLabel(CommandBufferHandle commandBufferHandle)
    {
        s_RendererCommands->EndLabel(commandBufferHandle);
    }


    void RendererAPI::InsertMarker(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color)
    {
        s_RendererCommands->InsertMarker(commandBufferHandle, label, color);
    }


    void RendererAPI::NameObject(BufferHandle bufferHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(bufferHandle, name);
    }


    void RendererAPI::NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(descriptorSetHandle, name);
    }


    void RendererAPI::NameObject(TextureHandle textureHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(textureHandle, name);
    }


    void RendererAPI::NameObject(ShaderHandle shaderHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(shaderHandle, name);
    }


    void RendererAPI::NameObject(FramebufferHandle framebufferHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(framebufferHandle, name);
    }


    void RendererAPI::NameObject(CommandBufferHandle commandBufferHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(commandBufferHandle, name);
    }


    void RendererAPI::NameObject(RenderPassHandle renderPassHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(renderPassHandle, name);
    }


    void RendererAPI::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        s_RendererCommands->CallImGuiDraws(commandBufferHandle);
    }

}
