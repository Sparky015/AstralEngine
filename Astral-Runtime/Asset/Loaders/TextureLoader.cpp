/**
* @file TextureLoader.cpp
* @author Andrew Fagan
* @date 4/13/25
*/

#include "TextureLoader.h"

#include "Core/SmartPointers.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    Ref<Asset> TextureLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("TextureLoader::LoadAsset")

        Ref<Texture> texture = Texture::CreateTexture(filePath.string());
        if (!texture) { return nullptr; } // Return nullptr if an error occured
        RendererAPI::NameObject(texture, filePath.filename().string().data());
        return texture;
    }

}
