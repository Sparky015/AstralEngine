/**
* @file TextureLoader.cpp
* @author Andrew Fagan
* @date 4/13/25
*/

#include "TextureLoader.h"

#include "Core/SmartPointers.h"
#include "Renderer/Primitives/Texture.h"

namespace Astral {

    Ref<Asset> TextureLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        Ref<Texture> texture = Texture::CreateTexture(filePath);
        if (!texture) { return nullptr; } // Return nullptr if an error occured

        if (!texture->IsValid())
        {
            texture.reset(); // Free the pointer
            return nullptr;
        }

        return texture;
    }

}
