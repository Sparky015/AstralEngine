//
// Created by Andrew Fagan on 11/29/24.
//

#include "RendererContext.h"

#include "Renderer.h"
#include "Window/WindowManager.h"

#include "Platform/OpenGL/OpenGLRenderingContext.h"

namespace Renderer {

    RendererContext* RendererContext::CreateRendererContext()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL:
                return new OpenGLRenderingContext((GLFWwindow*)nativeWindow);
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}