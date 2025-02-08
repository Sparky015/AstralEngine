//
// Created by Andrew Fagan on 11/30/24.
//

#include "Texture.h"

#include "Debug/Macros/Error.h"
#include "Renderer/Platform/OpenGL/Primitives/OpenGLTexture.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    Texture* Texture::CreateTexture(const std::string& filePath)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLTexture(filePath);
            case API::Vulkan: ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

} // Renderer