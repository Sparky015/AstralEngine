//
// Created by Andrew Fagan on 11/30/24.
//

#include "Texture.h"

#include "Renderer.h"
#include "Renderer/Platform/OpenGL/OpenGLTexture.h"

namespace Renderer {

    Texture* Texture::CreateTexture(const std::string& filePath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL: return new OpenGLTexture(filePath);
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
        }
    }

} // Renderer