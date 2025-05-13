//
// Created by Andrew Fagan on 11/29/24.
//

#include "RenderingContext.h"

#include "Debug/Utilities/Error.h"
#include "Platform/OpenGL/OpenGLRenderingContext.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Platform/Vulkan/VulkanRenderingContext.h"
#include "Window/WindowManager.h"

namespace Astral {

    RenderingContext* RenderingContext::CreateRendererContext()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLRenderingContext((GLFWwindow*)nativeWindow);
            case API::Vulkan: return new VulkanRenderingContext((GLFWwindow*)nativeWindow);
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            case API::None: ASTRAL_ERROR("No Rendering API was set!");
            default: ASTRAL_ERROR("Invalid Rendering API");
        }
    }

}
