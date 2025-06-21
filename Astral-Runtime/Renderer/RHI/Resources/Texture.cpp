//
// Created by Andrew Fagan on 11/30/24.
//

#include "Texture.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererCommands.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

#include "stb_image.h"


namespace Astral {

    TextureHandle Texture::CreateTexture(const std::string_view& filePath)
    {
        int m_Width;
        int m_Height;
        int m_BPP; // bytes per pixel
        unsigned char* data = stbi_load(filePath.data(), &m_Width, &m_Height, &m_BPP, 4);

        if (!data)
        {
            WARN("Failed to load texture! File path: " << filePath)
            return nullptr;
        }

        return Texture::CreateTexture(data, m_Width, m_Height);
    }


    GraphicsRef<Texture> Texture::CreateTexture(void* data, uint32 width, uint32 height)
    {
        ASSERT(data != nullptr, "Tried to create texture with nullptr!")
        ASSERT(width != 0, "Tried to create texture with width of zero!")
        ASSERT(height != 0, "Tried to create texture with height of zero!")

        if (!data)
        {
            WARN("Failed to load texture!")
            return nullptr;
        }

        TextureCreateInfo textureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = UVec2(width, height),
            .ImageData = (unsigned char*)data,
        };

        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateTexture(textureCreateInfo);
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer