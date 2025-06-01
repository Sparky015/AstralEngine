//
// Created by Andrew Fagan on 11/29/24.
//

#include "RendererContext.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Platform/OpenGL/OpenGLRendererContext.h"
#include "Platform/Vulkan/VulkanRendererContext.h"
#include "Renderer/RHI/RendererCommands.h"
#include "Window/WindowManager.h"

namespace Astral {

    RenderingContext* RenderingContext::CreateRendererContext()
    {
        Astral::Window& window = Engine::Get().GetWindowManager().GetWindow();
        void* nativeWindow = window.GetNativeWindow();

        switch (RendererCommands::GetAPI())
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
