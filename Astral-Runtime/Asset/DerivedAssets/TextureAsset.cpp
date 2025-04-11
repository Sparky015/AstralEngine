/**
* @file TextureAsset.cpp
* @author Andrew Fagan
* @date 4/11/25
*/

#include "TextureAsset.h"

#include "Debug/Utilities/Loggers.h"

#include "stb_image.h"

namespace Astral {

    AssetErrorCode TextureAsset::LoadData(std::filesystem::path filePath)
    {
        m_Texture = Texture::CreateTexture(filePath);

        if (m_Texture == nullptr) { return AssetErrorCode::FAILED_TO_LOAD_RESOURCE; }

        return AssetErrorCode::SUCCESS;
    }

    AssetType Astral::TextureAsset::GetAssetType()
    {
        return AssetType::Texture;
    }


    int TextureAsset::GetWidth()
    {
        m_Texture->GetHeight();
    }


    int TextureAsset::GetHeight()
    {
    }


    void TextureAsset::Bind()
    {
    }


    void TextureAsset::Unbind()
    {
    }
}

