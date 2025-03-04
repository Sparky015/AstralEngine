//
// Created by Andrew Fagan on 11/29/24.
//

#include "RenderingContext.h"

#include "Window/WindowManager.h"

#include "Platform/OpenGL/OpenGLRenderingContext.h"
#include "Renderer/AbstractionLayer/RendererAPI.h"
#include "Debug/Utilities/Error.h"

namespace Graphics {

    RenderingContext* RenderingContext::CreateRendererContext()
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
