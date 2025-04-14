/**
* @file TextureLoader.cpp
* @author Andrew Fagan
* @date 4/13/25
*/

#include "TextureLoader.h"

#include "Renderer/Primitives/Texture.h"

namespace Astral {

    Asset* TextureLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        Texture* texture = Texture::CreateTexture(filePath);
        if (!texture) { return texture; } // Return nullptr if an error occured

        if (!texture->IsValid())
        {
            delete(texture);
            return nullptr;
        }

        return texture;
    }

}
