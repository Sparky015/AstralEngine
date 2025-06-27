//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Asset/Asset.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/ImageUsageFlags.h"

#include <string>
#include <filesystem>
#include "imgui/imgui.h"


namespace Astral {

    struct TextureCreateInfo
    {
        ImageFormat Format;
        ImageLayout Layout;
        ImageUsageFlags UsageFlags;
        UVec2 Dimensions;
        uint8* ImageData;
    };

    class Texture : public Asset
    {
    public:
        ~Texture() override = default;

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual UVec2 GetDimensions() = 0;

        virtual ImageLayout GetLayout() = 0;
        virtual ImageFormat GetFormat() = 0;

        virtual void* GetSampler() = 0;
        virtual void* GetNativeHandle() = 0;

        virtual ImTextureID GetImGuiTextureID() = 0;

        static GraphicsRef<Texture> CreateTexture(const std::filesystem::path& filePath);
        static GraphicsRef<Texture> CreateTexture(void* data, uint32 width, uint32 height, ImageFormat imageFormat);

        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Texture; }
    };

    using TextureHandle = GraphicsRef<Texture>;

} // Renderer
