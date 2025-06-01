/**
* @file MaterialLoader.cpp
* @author Andrew Fagan
* @date 6/1/25
*/

#include "MaterialLoader.h"

#include "Asset/AssetManager.h"
#include "Asset/AssetRegistry.h"
#include "Core/Engine.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/Common/Material.h"
// #include "Core/Memory/Allocators/"

#include <fstream>

#include "Debug/Instrumentation/ScopeProfiler.h"

namespace Astral::MaterialLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MaterialLoader::LoadAsset")
        if (filePath.extension() != ".astmat") { ASTRAL_ERROR("Tried to load material file with wrong extension"); }

        std::ifstream fileStream = std::ifstream(filePath);

        std::string vertexShaderPath;
        std::string fragmentShaderPath;
        std::string texturePath;
        std::getline(fileStream, vertexShaderPath);
        std::getline(fileStream, fragmentShaderPath);
        std::getline(fileStream, texturePath);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        AssetID vertexShaderID = registry.CreateAsset<Shader>(vertexShaderPath);
        AssetID fragmentShaderID = registry.CreateAsset<Shader>(fragmentShaderPath);
        AssetID textureShaderID = registry.CreateAsset<Texture>(texturePath);

        // Material* materialPtr = new Material();
        Ref<Material> material = CreateRef<Material>();
        material->VertexShaderID = vertexShaderID;
        material->PixelShaderID = fragmentShaderID;
        material->TextureID = textureShaderID;

        return material;
    }

}
