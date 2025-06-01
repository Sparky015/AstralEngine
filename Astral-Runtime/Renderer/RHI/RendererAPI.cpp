//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Platform/Vulkan/VulkanRendererCommands.h"
#include "Renderer/RendererManager.h"


namespace Astral {

    RendererCommands* RendererAPI::s_RendererCommands = new VulkanRendererCommands();


    Device& RendererAPI::GetDevice()
    {
        return g_RendererManager.GetContext().GetDevice();
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


    void RendererAPI::PushConstants(CommandBufferHandle commandBuffer, PipelineStateObjectHandle pipelineStateObjectHandle, void* data, uint32 sizeInBytes)
    {
        s_RendererCommands->PushConstants(commandBuffer, pipelineStateObjectHandle, data, sizeInBytes);
    }


    void RendererAPI::SetBlending(bool enable)
    {
        s_RendererCommands->SetBlending(enable);
    }


    void RendererAPI::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        s_RendererCommands->CallImGuiDraws(commandBufferHandle);
    }

}
