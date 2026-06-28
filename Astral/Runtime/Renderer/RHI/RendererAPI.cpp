//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Core/Engine.h"
#include "Platform/Metal/MetalRendererCommands.h"
#include "Platform/Vulkan/VulkanRendererCommands.h"
#include "Renderer/RendererManager.h"


namespace Astral {

#ifdef ASTRAL_DEFAULT_RENDERING_API_VULKAN
    RendererCommands* RendererAPI::s_RendererCommands = new VulkanRendererCommands();
#elif ASTRAL_DEFAULT_RENDERING_API_METAL
    RendererCommands* RendererAPI::s_RendererCommands = new MetalRendererCommands();
#endif


    Device& RendererAPI::GetDevice()
    {
        return Engine::Get().GetRendererManager().GetContext().GetDevice();
    }


    RenderingContext& RendererAPI::GetContext()
    {
        return Engine::Get().GetRendererManager().GetContext();
    }


    void RendererAPI::ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback)
    {
        s_RendererCommands->ExecuteOneTimeAndBlock(callback);
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


    void RendererAPI::NameObject(CommandBufferHandle commandBufferHandle, std::string_view name)
    {
        s_RendererCommands->NameObject(commandBufferHandle, name);
    }


    void RendererAPI::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        s_RendererCommands->CallImGuiDraws(commandBufferHandle);
    }


    const RendererDebugStats& RendererAPI::GetRendererDebugStats()
    {
        return s_RendererCommands->GetRendererDebugStats();
    }


    RendererDebugStats& RendererAPI::GetInProgressRendererDebugStats()
    {
        return s_RendererCommands->GetInProgressRendererDebugStats();
    }

}
