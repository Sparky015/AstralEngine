//
// Created by Andrew Fagan on 11/29/24.
//

#include "RendererContext.h"

#include "Window/WindowManager.h"

#include "Platform/OpenGL/OpenGLRenderingContext.h"
#include "RendererAPI.h"
#include "../Debug/Macros/Error.h"

namespace Graphics {

    RendererContext* RendererContext::CreateRendererContext()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLRenderingContext((GLFWwindow*)nativeWindow);
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}
