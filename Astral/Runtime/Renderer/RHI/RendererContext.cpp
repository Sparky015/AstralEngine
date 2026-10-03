//
// Created by Andrew Fagan on 11/29/24.
//

#include "RendererContext.h"

#include "Core/Engine.h"
#include "Core/Utilities/Error.h"
#include "Platform/Vulkan/VulkanRendererContext.h"
#include "Renderer/RHI/RendererCommands.h"
#include "Window/WindowManager.h"

#ifdef ASTRAL_METAL_AVAILABLE
#include "Platform/Metal/MetalRendererContext.h"
#endif

#ifdef ASTRAL_VULKAN_AVAILABLE
#include "Platform/Vulkan/VulkanRendererContext.h"
#endif


namespace Astral {

    UVec2 RenderingContext::GetWindowFramebufferDimensions()
    {
        Astral::Window& window = Engine::Get().GetWindowManager().GetWindow();
        return window.GetFramebufferDimensions();
    }


    RenderingContext* RenderingContext::CreateRendererContext()
    {
        Astral::Window& window = Engine::Get().GetWindowManager().GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (RendererCommands::GetAPI())
        {
#ifdef ASTRAL_VULKAN_AVAILABLE
            case API::Vulkan: return new VulkanRenderingContext((GLFWwindow*)nativeWindow);
#endif
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
#ifdef ASTRAL_METAL_AVAILABLE
            case API::Metal: return new MetalRenderingContext((GLFWwindow*)nativeWindow);
#endif
            case API::None: AE_ERROR("No Rendering API was set!");
            default: AE_ERROR("Invalid Rendering API");
        }
    }

}
