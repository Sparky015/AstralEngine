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
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

#include "stb_image.h"
#include "gli/gli/gli.hpp"

namespace Astral {

    ImageFormat ConvertGliImageFormatToAstralImageFormat(gli::format imageFormat);


    uint32 Texture::CalculateMipMapLevels(uint32 width, uint32 height)
    {
        if (width == 0 || height == 0) return 1;
        return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
    }


    uint32 Texture::CalculateMipMapLevelSize(ImageFormat imageFormat, uint32 width, uint32 height, uint32 depth, uint32 numLayers)
    {
        uint32 levelSize = 0;
        if (IsCompressed(imageFormat))
        {
            uint32 bytesPerBlock = GetBytesPerTexel(imageFormat);
            Vec2 blockExtent = GetCompressedFormatBlockExtent(imageFormat);
            uint32 numBlocksX = (width + blockExtent.x - 1) / blockExtent.x;
            uint32 numBlocksY = (height + blockExtent.y - 1) / blockExtent.y;
            uint32 layerSize = numBlocksX * numBlocksY * bytesPerBlock;
            levelSize = numLayers * layerSize;
        }
        else
        {
            uint32 bytesPerPixel = GetBytesPerTexel(imageFormat);
            uint32 layerSize = width * height * depth * bytesPerPixel;
            levelSize = numLayers * layerSize;
        }

        return levelSize;
    }


    TextureHandle Texture::CreateTexture(const std::filesystem::path& filePath)
    {
        int width;
        int height;
        int bpp; // bytes per pixel
        ImageFormat imageFormat;
        unsigned char* data;

        TextureCreateInfo textureCreateInfo;
        gli::texture texture; // Only used if the file extension is .dds or .ktx


        if (filePath.extension() == ".dds" || filePath.extension() == ".ktx")
        {
            texture = gli::load(filePath.string().c_str());

            if (texture.target() != gli::TARGET_2D)
            {
                AE_WARN("Loaded texture is not a 2D image!: " << filePath)
                return nullptr;
            }

            gli::extent2d extent = texture.extent();
            width = extent.x;
            height = extent.y;
            data = (unsigned char*)texture.data();
            gli::format gliImageFormat = texture.format();
            imageFormat = ConvertGliImageFormatToAstralImageFormat(gliImageFormat);
            if (texture.empty()) { data = nullptr; }

            textureCreateInfo.ImageDataLength = texture.size();
            textureCreateInfo.LayerCount = texture.layers();
            textureCreateInfo.MipMapCount = texture.levels();
            textureCreateInfo.GenerateMipMaps = false;
        }
        else
        {
            data = stbi_load(filePath.string().c_str(), &width, &height, &bpp, 4);
            imageFormat = ImageFormat::R8G8B8A8_SRGB;

            textureCreateInfo.ImageDataLength = width * height * bpp;
            textureCreateInfo.LayerCount = 1;
            textureCreateInfo.MipMapCount = Texture::CalculateMipMapLevels(width, height);
            textureCreateInfo.GenerateMipMaps = true;
        }

        if (!data)
        {
            AE_WARN("Failed to load texture! File path: " << filePath)
            return nullptr;
        }

        textureCreateInfo.Format = imageFormat;
        textureCreateInfo.Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
        textureCreateInfo.UsageFlags = IMAGE_USAGE_SAMPLED_BIT;
        textureCreateInfo.Dimensions.x = width;
        textureCreateInfo.Dimensions.y = height;
        textureCreateInfo.ImageData = data;


        TextureHandle textureHandle = Texture::CreateTexture(textureCreateInfo);

        if (!(filePath.extension() == ".dds" || filePath.extension() == ".ktx"))
        {
            stbi_image_free(data);
        }

        return textureHandle;
    }


    GraphicsRef<Texture> Texture::CreateCubemap(const TextureCreateInfo& textureCreateInfo)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateCubemap(textureCreateInfo);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }


    GraphicsRef<Texture> Texture::CreateCubemap(void* data, uint32 width, uint32 height, ImageFormat imageFormat, ImageUsageFlags imageUsageFlags)
    {
        TextureCreateInfo textureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT | imageUsageFlags,
            .Dimensions = UVec2(width, height),
            .ImageData = (uint8*)data,
        };

        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateCubemap(textureCreateInfo);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }


    GraphicsRef<Texture> Texture::Create3DTexture(void* data, uint32 width, uint32 height, ImageFormat imageFormat)
    {
        TextureCreateInfo textureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = ImageUsageFlagBits::IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = UVec2(width, height), // depth is inferred from the width and height since all three should be the same
            .ImageData = (uint8*)data,
            .SamplerFilter = SamplerFilter::LINEAR,
            .SamplerAddressMode = SamplerAddressMode::CLAMP_TO_EDGE,
            .EnableAnisotropy = false,
        };

        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.Create3DTexture(textureCreateInfo);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }


    GraphicsRef<Texture> Texture::Create1DTexture(void* data, uint32 length, ImageFormat imageFormat)
    {
        TextureCreateInfo textureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = UVec2(length, length), // depth is inferred from the width and height since all three should be the same
            .ImageData = (uint8*)data,
            .SamplerFilter = SamplerFilter::LINEAR,
            .SamplerAddressMode = SamplerAddressMode::CLAMP_TO_EDGE,
            .EnableAnisotropy = false,
        };

        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.Create1DTexture(textureCreateInfo);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }


    GraphicsRef<Texture> Texture::CreateTexture(const TextureCreateInfo& textureCreateInfo)
    {
        ASSERT(textureCreateInfo.ImageData != nullptr, "Tried to create texture with nullptr!")
        ASSERT(textureCreateInfo.Dimensions.x != 0, "Tried to create texture with width of zero!")
        ASSERT(textureCreateInfo.Dimensions.y != 0, "Tried to create texture with height of zero!")

        if (!textureCreateInfo.ImageData)
        {
            AE_WARN("Failed to load texture!")
            return nullptr;
        }


        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateTexture(textureCreateInfo);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }


    ImageFormat ConvertGliImageFormatToAstralImageFormat(gli::format imageFormat)
    {
        switch (imageFormat)
        {
            case gli::FORMAT_R8_UNORM_PACK8:     return ImageFormat::R8_UNORM;
            case gli::FORMAT_R8_SNORM_PACK8:     return ImageFormat::R8_SNORM;
            case gli::FORMAT_R8_UINT_PACK8:      return ImageFormat::R8_UINT;
            case gli::FORMAT_R8_SINT_PACK8:      return ImageFormat::R8_SINT;
            case gli::FORMAT_R8_SRGB_PACK8:      return ImageFormat::R8_SRGB;

            // 8-bit two channel
            case gli::FORMAT_RG8_UNORM_PACK8:    return ImageFormat::R8G8_UNORM;
            case gli::FORMAT_RG8_SNORM_PACK8:    return ImageFormat::R8G8_SNORM;
            case gli::FORMAT_RG8_UINT_PACK8:     return ImageFormat::R8G8_UINT;
            case gli::FORMAT_RG8_SINT_PACK8:     return ImageFormat::R8G8_SINT;
            case gli::FORMAT_RG8_SRGB_PACK8:     return ImageFormat::R8G8_SRGB;

            // 8-bit three channel
            case gli::FORMAT_RGB8_UNORM_PACK8:   return ImageFormat::R8G8B8_UNORM;
            case gli::FORMAT_RGB8_SNORM_PACK8:   return ImageFormat::R8G8B8_SNORM;
            case gli::FORMAT_RGB8_UINT_PACK8:    return ImageFormat::R8G8B8_UINT;
            case gli::FORMAT_RGB8_SINT_PACK8:    return ImageFormat::R8G8B8_SINT;
            case gli::FORMAT_RGB8_SRGB_PACK8:    return ImageFormat::R8G8B8_SRGB;

            // 8-bit four channel
            case gli::FORMAT_RGBA8_UNORM_PACK8:  return ImageFormat::R8G8B8A8_UNORM;
            case gli::FORMAT_RGBA8_SNORM_PACK8:  return ImageFormat::R8G8B8A8_SNORM;
            case gli::FORMAT_RGBA8_UINT_PACK8:   return ImageFormat::R8G8B8A8_UINT;
            case gli::FORMAT_RGBA8_SINT_PACK8:   return ImageFormat::R8G8B8A8_SINT;
            case gli::FORMAT_RGBA8_SRGB_PACK8:   return ImageFormat::R8G8B8A8_SRGB;

            // 16-bit single channel
            case gli::FORMAT_R16_UNORM_PACK16:   return ImageFormat::R16_UNORM;
            case gli::FORMAT_R16_SNORM_PACK16:   return ImageFormat::R16_SNORM;
            case gli::FORMAT_R16_UINT_PACK16:    return ImageFormat::R16_UINT;
            case gli::FORMAT_R16_SINT_PACK16:    return ImageFormat::R16_SINT;
            case gli::FORMAT_R16_SFLOAT_PACK16:  return ImageFormat::R16_SFLOAT;

            // 16-bit two channel
            case gli::FORMAT_RG16_UNORM_PACK16:  return ImageFormat::R16G16_UNORM;
            case gli::FORMAT_RG16_SNORM_PACK16:  return ImageFormat::R16G16_SNORM;
            case gli::FORMAT_RG16_UINT_PACK16:   return ImageFormat::R16G16_UINT;
            case gli::FORMAT_RG16_SINT_PACK16:   return ImageFormat::R16G16_SINT;
            case gli::FORMAT_RG16_SFLOAT_PACK16: return ImageFormat::R16G16_SFLOAT;

            // 16-bit four channel
            case gli::FORMAT_RGBA16_UNORM_PACK16:  return ImageFormat::R16G16B16A16_UNORM;
            case gli::FORMAT_RGBA16_SNORM_PACK16:  return ImageFormat::R16G16B16A16_SNORM;
            case gli::FORMAT_RGBA16_UINT_PACK16:   return ImageFormat::R16G16B16A16_UINT;
            case gli::FORMAT_RGBA16_SINT_PACK16:   return ImageFormat::R16G16B16A16_SINT;
            case gli::FORMAT_RGBA16_SFLOAT_PACK16: return ImageFormat::R16G16B16A16_SFLOAT;

            // 32-bit single channel
            case gli::FORMAT_R32_UINT_PACK32:     return ImageFormat::R32_UINT;
            case gli::FORMAT_R32_SINT_PACK32:     return ImageFormat::R32_SINT;
            case gli::FORMAT_R32_SFLOAT_PACK32:   return ImageFormat::R32_SFLOAT;

            // 32-bit two channel
            case gli::FORMAT_RG32_UINT_PACK32:    return ImageFormat::R32G32_UINT;
            case gli::FORMAT_RG32_SINT_PACK32:    return ImageFormat::R32G32_SINT;
            case gli::FORMAT_RG32_SFLOAT_PACK32:  return ImageFormat::R32G32_SFLOAT;

            // 32-bit four channel
            case gli::FORMAT_RGBA32_UINT_PACK32:   return ImageFormat::R32G32B32A32_UINT;
            case gli::FORMAT_RGBA32_SINT_PACK32:   return ImageFormat::R32G32B32A32_SINT;
            case gli::FORMAT_RGBA32_SFLOAT_PACK32: return ImageFormat::R32G32B32A32_SFLOAT;

            // BC Compressed (DXTn)
            case gli::FORMAT_RGB_DXT1_UNORM_BLOCK8:   return ImageFormat::BC1_RGB_UNORM_BLOCK;
            case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8:    return ImageFormat::BC1_RGB_SRGB_BLOCK;
            case gli::FORMAT_RGBA_DXT1_UNORM_BLOCK8:  return ImageFormat::BC1_RGBA_UNORM_BLOCK;
            case gli::FORMAT_RGBA_DXT1_SRGB_BLOCK8:   return ImageFormat::BC1_RGBA_SRGB_BLOCK;
            case gli::FORMAT_RGBA_DXT3_UNORM_BLOCK16: return ImageFormat::BC2_UNORM_BLOCK;
            case gli::FORMAT_RGBA_DXT3_SRGB_BLOCK16:  return ImageFormat::BC2_SRGB_BLOCK;
            case gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16: return ImageFormat::BC3_UNORM_BLOCK;
            case gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16:  return ImageFormat::BC3_SRGB_BLOCK;
            case gli::FORMAT_R_ATI1N_UNORM_BLOCK8:    return ImageFormat::BC4_UNORM_BLOCK;
            case gli::FORMAT_R_ATI1N_SNORM_BLOCK8:    return ImageFormat::BC4_SNORM_BLOCK;
            case gli::FORMAT_RG_ATI2N_UNORM_BLOCK16:  return ImageFormat::BC5_UNORM_BLOCK;
            case gli::FORMAT_RG_ATI2N_SNORM_BLOCK16:  return ImageFormat::BC5_SNORM_BLOCK;
            case gli::FORMAT_RGB_BP_UFLOAT_BLOCK16:   return ImageFormat::BC6H_UFLOAT_BLOCK;
            case gli::FORMAT_RGB_BP_SFLOAT_BLOCK16:   return ImageFormat::BC6H_SFLOAT_BLOCK;
            case gli::FORMAT_RGBA_BP_UNORM_BLOCK16:   return ImageFormat::BC7_UNORM_BLOCK;
            case gli::FORMAT_RGBA_BP_SRGB_BLOCK16:    return ImageFormat::BC7_SRGB_BLOCK;

            // ETC2 Compressed
            case gli::FORMAT_RGB_ETC2_UNORM_BLOCK8:   return ImageFormat::ETC2_R8G8B8_UNORM_BLOCK;
            case gli::FORMAT_RGB_ETC2_SRGB_BLOCK8:    return ImageFormat::ETC2_R8G8B8_SRGB_BLOCK;
            case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK8:  return ImageFormat::ETC2_R8G8B8A1_UNORM_BLOCK;
            case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK8:   return ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK;
            case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK16: return ImageFormat::ETC2_R8G8B8A8_UNORM_BLOCK;
            case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK16:  return ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK;

            // EAC Compressed
            case gli::FORMAT_R_EAC_UNORM_BLOCK8:      return ImageFormat::EAC_R11_UNORM_BLOCK;
            case gli::FORMAT_R_EAC_SNORM_BLOCK8:      return ImageFormat::EAC_R11_SNORM_BLOCK;
            case gli::FORMAT_RG_EAC_UNORM_BLOCK16:    return ImageFormat::EAC_R11G11_UNORM_BLOCK;
            case gli::FORMAT_RG_EAC_SNORM_BLOCK16:    return ImageFormat::EAC_R11G11_SNORM_BLOCK;

            // ASTC Compressed
            case gli::FORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16:  return ImageFormat::ASTC_4x4_UNORM_BLOCK;
            case gli::FORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16:   return ImageFormat::ASTC_4x4_SRGB_BLOCK;

        default: AE_ERROR("No image format match was found!")
        }
    }

} // Renderer