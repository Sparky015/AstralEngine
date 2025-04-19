//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include <filesystem>

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"


namespace Astral {

    class Texture : public Asset
    {
    public:
        ~Texture() override = default;

        static AssetType GetStaticAssetType() { return AssetType::Texture; }
        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }

        virtual void Bind(unsigned int slot = 0) = 0;
        virtual void Unbind() = 0;
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual bool IsValid() = 0;

        static Ref<Texture> CreateTexture(const std::filesystem::path& filePath);
    };

} // Renderer
