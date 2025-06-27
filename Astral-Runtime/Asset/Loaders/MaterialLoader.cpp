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
#include "Renderer/RendererManager.h"

#include <fstream>

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral::MaterialLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MaterialLoader::LoadAsset")
        if (filePath.extension() != ".astmat") { ASTRAL_ERROR("Tried to load material file with wrong extension: " << filePath); }

        std::ifstream fileStream = std::ifstream(filePath);

        std::string vertexShaderPath;
        std::string fragmentShaderPath;
        std::string texturePath;
        std::getline(fileStream, vertexShaderPath);
        std::getline(fileStream, fragmentShaderPath);
        std::getline(fileStream, texturePath);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        Ref<Shader> vertexShader = registry.CreateAsset<Shader>(vertexShaderPath);
        Ref<Shader> fragmentShader = registry.CreateAsset<Shader>(fragmentShaderPath);
        Ref<Texture> texture = registry.CreateAsset<Texture>(texturePath);

        DescriptorSetHandle descriptorSetHandle = DescriptorSet::CreateDescriptorSet();
        descriptorSetHandle->BeginBuildingSet();
        descriptorSetHandle->AddDescriptorImageSampler(texture, ShaderStage::FRAGMENT);
        descriptorSetHandle->EndBuildingSet();

        RendererAPI::NameObject(descriptorSetHandle, filePath.filename().string().data());

        Ref<Material> material = CreateRef<Material>();
        material->VertexShader = vertexShader;
        material->FragmentShader = fragmentShader;
        material->Texture = texture;
        material->DescriptorSet = descriptorSetHandle;

        return material;
    }


    void SerializeMaterial(Ref<Material> material, std::filesystem::path& outFilePath)
    {
        outFilePath.replace_extension(".astmat");

        std::ofstream fileStream = std::ofstream(outFilePath);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        std::filesystem::path vertexShaderPath = registry.GetFilePathFromAssetID(material->VertexShader->GetAssetID());
        std::filesystem::path fragmentShaderPath = registry.GetFilePathFromAssetID(material->FragmentShader->GetAssetID());
        std::filesystem::path texturePath = registry.GetFilePathFromAssetID(material->Texture->GetAssetID());

        fileStream << vertexShaderPath.string() << "\n" << fragmentShaderPath.string() << "\n" << texturePath.string();
    }

}
