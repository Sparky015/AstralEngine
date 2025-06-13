//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Asset/Asset.h"

#include <string>

#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"

namespace Astral {

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

        static GraphicsRef<Texture> CreateTexture(const std::string_view& filePath);

        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Texture; }
    };

    using TextureHandle = GraphicsRef<Texture>;

} // Renderer
