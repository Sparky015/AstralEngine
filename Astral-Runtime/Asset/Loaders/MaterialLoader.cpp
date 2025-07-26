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

namespace Astral {

    Ref<Asset> MaterialLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MaterialLoader::LoadAsset")
        if (filePath.extension() != ".astmat") { ASTRAL_ERROR("Tried to load material file with wrong extension: " << filePath); }

        std::ifstream fileStream = std::ifstream(filePath);

        ASSERT(!fileStream.eof(), "Material file is empty!")

        Ref<Material> material = CreateRef<Material>();

        std::string shaderModel;
        std::getline(fileStream, shaderModel);

        material->ShaderModel = shaderModel == "PBR" ? ShaderModel::PBR : ShaderModel::UNLIT;

        std::string fragmentShaderPath;
        std::getline(fileStream, fragmentShaderPath);
        std::replace(fragmentShaderPath.begin(), fragmentShaderPath.end(), '\\', '/');
        std::string texturePath;
        std::getline(fileStream, texturePath);
        std::replace(texturePath.begin(), texturePath.end(), '\\', '/');

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
            std::replace(optional_metallic.begin(), optional_metallic.end(), '\\', '/');
            std::getline(fileStream, optional_roughness);
            std::replace(optional_roughness.begin(), optional_roughness.end(), '\\', '/');
            std::getline(fileStream, optional_emission);
            std::replace(optional_emission.begin(), optional_emission.end(), '\\', '/');
            std::getline(fileStream, optional_normals);
            std::replace(optional_normals.begin(), optional_normals.end(), '\\', '/');

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

        material->FragmentShader = fragmentShader;
        material->Textures.push_back(texture);
        material->DescriptorSet = descriptorSetHandle;
        material->HasNormalMap = false; // TODO: Write this to the material file or have a normal map field with no file path assigned

        return material;
    }


    void MaterialLoader::SerializeMaterial(Ref<Material> material, std::filesystem::path& outFilePath)
    {
        outFilePath.replace_extension(".astmat");
        std::ofstream fileStream = std::ofstream(outFilePath);

        fileStream << ShaderModelToString(material->ShaderModel) << "\n";

        // TODO: serialize the texture conventions
        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        std::string fragmentShaderPath = registry.GetFilePathFromAssetID(material->FragmentShader->GetAssetID()).generic_string();

        fileStream << fragmentShaderPath << "\n";

        ASSERT(!material->Textures.empty(), "Expected material to have at least 1 texture!")
        std::filesystem::path baseColorPath = registry.GetFilePathFromAssetID(material->Textures[0]->GetAssetID());
        fileStream << baseColorPath.generic_string() << "\n";

        if (material->ShaderModel == ShaderModel::PBR)
        {
            ASSERT(material->Textures.size() >= 4, "Expected PBR material to have at least 4 maps!")
            for (int i = 1; i < material->Textures.size(); i++)
            {
                std::filesystem::path mapPath = registry.GetFilePathFromAssetID(material->Textures[i]->GetAssetID());
                fileStream << mapPath.generic_string() << "\n";
            }
        }
    }

}
