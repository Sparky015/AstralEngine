//
// Created by Andrew Fagan on 11/29/24.
//

#include "RendererContext.h"

#include "Window/WindowManager.h"

#include "Platform/OpenGL/OpenGLRenderingContext.h"
#include "RendererAPI.h"

namespace Graphics {

    RendererContext* RendererContext::CreateRendererContext()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLRenderingContext((GLFWwindow*)nativeWindow);
            case API::Vulkan: ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}