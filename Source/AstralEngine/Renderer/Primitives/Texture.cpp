//
// Created by Andrew Fagan on 11/30/24.
//

#include "Texture.h"

#include "Debug/Utils/Public/Error.h"
#include "Renderer/Platform/OpenGL/Primitives/OpenGLTexture.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    Texture* Texture::CreateTexture(const std::string& filePath)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLTexture(filePath);
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer