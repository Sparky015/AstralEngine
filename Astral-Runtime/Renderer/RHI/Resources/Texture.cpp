//
// Created by Andrew Fagan on 11/30/24.
//

#include "Texture.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererCommands.h"

#include "stb_image.h"

namespace Astral {

    TextureHandle Texture::CreateTexture(const std::string_view& filePath)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        int m_Width;
        int m_Height;
        int m_BPP; // bytes per pixel
        unsigned char* data = stbi_load(filePath.data(), &m_Width, &m_Height, &m_BPP, 4);

        if (!data) { return nullptr; }

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateTexture(ImageFormat::R8G8B8A8_UNORM, ImageLayout::SHADER_READ_ONLY_OPTIMAL, UVec2(m_Width, m_Height), data);
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer