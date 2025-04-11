/**
* @file TextureAsset.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Renderer/Primitives/Texture.h"

namespace Astral {

    class TextureAsset : public Asset
    {
    public:
        AssetErrorCode LoadData(std::filesystem::path filePath) override;
        AssetType GetAssetType() override;

        int GetWidth();
        int GetHeight();
        void Bind();
        void Unbind();

    private:

        Graphics::Texture* m_Texture{nullptr};
    };

}
