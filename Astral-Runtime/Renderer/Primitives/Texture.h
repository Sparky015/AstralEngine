//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include <string>
#include "Asset/Asset.h"

namespace Astral {

    class Texture : public Asset
    {
    public:
        ~Texture() override = default;

        AssetErrorCode LoadData(std::filesystem::path filePath) override = 0;
        AssetType GetAssetType() override { return AssetType::Texture; }

        virtual void Bind(unsigned int slot = 0) = 0;
        virtual void Unbind() = 0;
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;

        static Texture* CreateTexture();
    };

} // Renderer
