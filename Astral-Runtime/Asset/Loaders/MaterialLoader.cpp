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

#include "yaml-cpp/yaml.h"

namespace Astral {

    Ref<Asset> MaterialLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MaterialLoader::LoadAsset")
        if (filePath.extension() != ".astmat") { AE_ERROR("Tried to load material file with wrong extension: " << filePath); }

        std::ifstream fileStream = std::ifstream(filePath);
        ASSERT(!fileStream.eof(), "Material file is empty!")
        YAML::Node file = YAML::Load(fileStream);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        Ref<Material> material = CreateRef<Material>();

        try {
            std::string shaderModel = file["Shader Model"].as<std::string>();
            material->ShaderModel = StringToShaderModel(shaderModel);

            std::string shaderPath = file["Shader"].as<std::string>();
            std::replace(shaderPath.begin(), shaderPath.end(), '\\', '/');
            material->FragmentShader = registry.CreateAsset<Shader>(shaderPath);

            std::string textureConvention = file["Texture Convention"].as<std::string>();
            material->TextureConvention = StringToTextureConvention(textureConvention);

            std::string normalsConvention = file["Normals Convention"].as<std::string>();
            material->HasDirectXNormals = normalsConvention == "DirectX";

            material->IsAlphaBlended = file["Is Alpha Blended"].as<bool>();


            DescriptorSetHandle descriptorSetHandle = DescriptorSet::CreateDescriptorSet();
            descriptorSetHandle->BeginBuildingSet();

            const YAML::Node& textureMaps = file["Texture Maps"];
            for (const auto& map : textureMaps)
            {
                std::string textureMapPath = map.begin()->second.as<std::string>();
                std::replace(textureMapPath.begin(), textureMapPath.end(), '\\', '/');

                if (map.begin()->first.as<std::string>() == "Normals")
                {
                    if (textureMapPath == "")
                    {
                        material->HasNormalMap = false;
                        textureMapPath = "Textures/SolidWhite.png";
                    }
                }

                TextureHandle textureMap = registry.CreateAsset<Texture>(textureMapPath);
                material->Textures.push_back(textureMap);
                descriptorSetHandle->AddDescriptorImageSampler(textureMap, ShaderStage::FRAGMENT);
            }

            descriptorSetHandle->EndBuildingSet();
            RendererAPI::NameObject(descriptorSetHandle, filePath.filename().generic_string().data());
            material->DescriptorSet = descriptorSetHandle;
        }
        catch (const YAML::Exception& e)
        {
            AE_ERROR("Error during material loading! Error: " << e.what())
            return nullptr;
        }
        catch (...)
        {
            AE_ERROR("Error during material loading!")
            return nullptr;
        }

        return material;
    }


    void MaterialLoader::SerializeMaterial(Ref<Material> material, std::filesystem::path& outFilePath)
    {
        outFilePath.replace_extension(".astmat");
        std::ofstream fileStream = std::ofstream(outFilePath);
        YAML::Emitter out{fileStream};

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();


        out << YAML::BeginMap;

        out << YAML::Key << "Shader Model";
        out << YAML::Value << ShaderModelToString(material->ShaderModel).data();

        out << YAML::Key << "Shader";
        out << YAML::Value << registry.GetFilePathFromAssetID(material->FragmentShader->GetAssetID()).generic_string();

        out << YAML::Key << "Texture Convention";
        out << YAML::Value << TextureConventionToString(material->TextureConvention).data();

        out << YAML::Key << "Normals Convention";
        out << YAML::Value << (material->HasDirectXNormals ? "DirectX" : "Vulkan/OpenGL");

        out << YAML::Key << "Is Alpha Blended";
        out << YAML::Value << material->IsAlphaBlended;


        out << YAML::Key << "Texture Maps";
        out << YAML::Value << YAML::BeginSeq;

        if (material->ShaderModel == ShaderModel::PBR)
        {
            if (material->TextureConvention == TextureConvention::UNPACKED)
            {
                ASSERT(material->Textures.size() == 5, "Expected PBR material with unpacked texture convention to have at least 5 maps!")
                static constexpr std::array<std::string_view, 5> textureNames = {"Base Color", "Metallic", "Roughness", "Emission", "Normals"};

                for (size_t i = 0; i < material->Textures.size(); i++)
                {
                    AssetID textureAssetID = material->Textures[i]->GetAssetID();
                    std::filesystem::path mapPath = registry.GetFilePathFromAssetID(textureAssetID);

                    if (i == 4) // Normals texture
                    {
                        if (!material->HasNormalMap) { mapPath = ""; }
                    }

                    out << YAML::BeginMap;
                    out << YAML::Key << textureNames[i].data();
                    out << YAML::Value << mapPath.generic_string();
                    out << YAML::EndMap;
                }
            }
            else if (material->TextureConvention == TextureConvention::ORM_PACKED)
            {
                ASSERT(material->Textures.size() == 4, "Expected PBR material with ao-roughness-metallic packed texture convention to have at least 4 maps!")
                static constexpr std::array<std::string_view, 4> textureNames = {"Base Color", "AO-Roughness-Metallic", "Emission", "Normals"};

                for (size_t i = 0; i < material->Textures.size(); i++)
                {
                    AssetID textureAssetID = material->Textures[i]->GetAssetID();
                    std::filesystem::path mapPath = registry.GetFilePathFromAssetID(textureAssetID);

                    if (i == 3) // Normals texture
                    {
                        if (!material->HasNormalMap) { mapPath = ""; }
                    }

                    out << YAML::BeginMap;
                    out << YAML::Key << textureNames[i].data();
                    out << YAML::Value << mapPath.generic_string();
                    out << YAML::EndMap;
                }
            }
        }
        else if (material->ShaderModel == ShaderModel::UNLIT)
        {
            ASSERT(material->Textures.size() == 1, "Expected Unlit material to have 1 texture!")
            AssetID textureAssetID = material->Textures[0]->GetAssetID();
            std::filesystem::path mapPath = registry.GetFilePathFromAssetID(textureAssetID);
            out << YAML::BeginMap;
            out << YAML::Key << "Base Color";
            out << YAML::Value << mapPath.generic_string();
            out << YAML::EndMap;
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        fileStream.close();
    }

}
