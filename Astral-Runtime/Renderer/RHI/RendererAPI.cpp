//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Platform/Vulkan/VulkanRendererCommands.h"


namespace Astral {

    RendererCommands* RendererAPI::s_RendererCommands = new VulkanRendererCommands();

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


    void RendererAPI::SetBlending(bool enable)
    {
        s_RendererCommands->SetBlending(enable);
    }


    void RendererAPI::TestInit()
    {
        s_RendererCommands->TestInit();
    }


    void RendererAPI::TestUpdate()
    {
        s_RendererCommands->TestUpdate();
    }


    void RendererAPI::TestShutdown()
    {
        s_RendererCommands->TestShutdown();
    }

}
