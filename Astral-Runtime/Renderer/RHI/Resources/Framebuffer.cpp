/**
* @file Framebuffer.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "Framebuffer.h"

#include "Renderer/RHI/RendererCommands.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    Framebuffer* Framebuffer::CreateFramebuffer()
    {
        switch (RendererCommands::GetAPI())
        {
            case API::OpenGL: ASTRAL_ERROR("OpenGL is not supported yet!");
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}