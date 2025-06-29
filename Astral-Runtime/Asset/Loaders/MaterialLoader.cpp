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
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"

#include <fstream>

namespace Astral::MaterialLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MaterialLoader::LoadAsset")
        if (filePath.extension() != ".astmat") { ASTRAL_ERROR("Tried to load material file with wrong extension: " << filePath); }

        std::ifstream fileStream = std::ifstream(filePath);

        ASSERT(!fileStream.eof(), "Material file is empty!")

        std::string fragmentShaderPath;
        std::string texturePath;
        std::getline(fileStream, fragmentShaderPath);
        std::getline(fileStream, texturePath);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        Ref<Shader> fragmentShader = registry.CreateAsset<Shader>(fragmentShaderPath);
        Ref<Texture> texture = registry.CreateAsset<Texture>(texturePath);
        if (!texture) { texture = registry.GetAsset<Texture>("Textures/MissingTexture.png"); }

        std::string optional_metallic;
        std::string optional_roughness;
        std::string optional_emission;
        std::string optional_normals;
        Ref<Texture> texture_metallic;
        Ref<Texture> texture_roughness;
        Ref<Texture> texture_emission;
        Ref<Texture> texture_normals;
        if (!fileStream.eof())
        {
            std::getline(fileStream, optional_metallic);
            std::getline(fileStream, optional_roughness);
            std::getline(fileStream, optional_emission);
            std::getline(fileStream, optional_normals);

            texture_metallic = registry.CreateAsset<Texture>(optional_metallic);
            texture_roughness = registry.CreateAsset<Texture>(optional_roughness);
            texture_emission = registry.CreateAsset<Texture>(optional_emission);
            texture_normals = registry.CreateAsset<Texture>(optional_normals);
        }


        DescriptorSetHandle descriptorSetHandle = DescriptorSet::CreateDescriptorSet();
        descriptorSetHandle->BeginBuildingSet();
        descriptorSetHandle->AddDescriptorImageSampler(texture, ShaderStage::FRAGMENT);
        if (optional_metallic != "")
        {
            descriptorSetHandle->AddDescriptorImageSampler(texture_metallic, ShaderStage::FRAGMENT);
            descriptorSetHandle->AddDescriptorImageSampler(texture_roughness, ShaderStage::FRAGMENT);
            descriptorSetHandle->AddDescriptorImageSampler(texture_emission, ShaderStage::FRAGMENT);
            descriptorSetHandle->AddDescriptorImageSampler(texture_normals, ShaderStage::FRAGMENT);
        }
        descriptorSetHandle->EndBuildingSet();

        RendererAPI::NameObject(descriptorSetHandle, filePath.filename().generic_string().data());

        Ref<Material> material = CreateRef<Material>();
        material->FragmentShader = fragmentShader;
        material->Textures.push_back(texture);
        material->DescriptorSet = descriptorSetHandle;

        return material;
    }


    void SerializeMaterial(Ref<Material> material, std::filesystem::path& outFilePath)
    {
        outFilePath.replace_extension(".astmat");
        std::ofstream fileStream = std::ofstream(outFilePath);

        fileStream << ShaderModelToString(material->ShaderModel) << "\n";


        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        std::string fragmentShaderPath = registry.GetFilePathFromAssetID(material->FragmentShader->GetAssetID()).generic_string();

        fileStream << fragmentShaderPath << "\n";

        ASSERT(!material->Textures.empty(), "Expected material to have at least 1 texture!")
        std::filesystem::path baseColorPath = registry.GetFilePathFromAssetID(material->Textures[0]->GetAssetID());
        fileStream << baseColorPath.generic_string();

        if (material->ShaderModel == ShaderModel::PBR)
        {
            ASSERT(material->Textures.size() >= 5, "Expected PBR material to have at least 5 maps!")
            for (int i = 1; i < material->Textures.size(); i++)
            {
                std::filesystem::path mapPath = registry.GetFilePathFromAssetID(material->Textures[i]->GetAssetID());
                fileStream << mapPath.generic_string();
            }
        }
    }

}
