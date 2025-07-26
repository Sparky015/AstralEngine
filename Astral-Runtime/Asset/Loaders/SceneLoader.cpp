/**
* @file SceneLoader.cpp
* @author Andrew Fagan
* @date 6/20/2025
*/

#include "SceneLoader.h"

#include "Debug/Utilities/Loggers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MaterialLoader.h"
#include "MeshLoader.h"
#include "Asset/AssetManager.h"
#include "assimp/Exporter.hpp"
#include "Core/Engine.h"
#include "Core/SmartPointers.h"
#include "ECS/SceneManager.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    ShaderModel SceneLoader::m_DefaultMaterialShaderModal = ShaderModel::PBR;
    TextureConvention SceneLoader::m_DefaultMaterialTextureConvention = TextureConvention::ORM_PACKED;

    TextureHandle SceneLoader::m_DefaultMaterialBaseColor = nullptr;
    TextureHandle SceneLoader::m_DefaultMaterialNormals = nullptr;
    TextureHandle SceneLoader::m_DefaultMaterialRoughness = nullptr;
    TextureHandle SceneLoader::m_DefaultMaterialMetallic = nullptr;
    TextureHandle SceneLoader::m_DefaultMaterialEmission = nullptr;


    void SceneLoader::LoadSceneAssets(const std::filesystem::path& sceneFilePath)
    {
        if (sceneFilePath.extension() != ".aescene")
        {
            WARN("Skipping the loading of the scene! File extension does not match the Astral Scene extension (.aescene)!")
            return;
        }

        Assimp::Importer importer;
        const aiScene* importedScene = importer.ReadFile( sceneFilePath.string(), 0);
        ASSERT(importedScene != nullptr, "Scene file parsing failed!")

        SceneManager& sceneManager = Engine::Get().GetSceneManager();
        Scene& activeScene = sceneManager.GetActiveScene();
        ECS& sceneECS = Engine::Get().GetSceneManager().GetECS();
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        // Load external resources
        aiMetadata* sceneMetaData = importedScene->mMetaData;
        std::unordered_map<SceneResourceID, std::string> externalResourceIDMapping;
        if (sceneMetaData->HasKey("AE_ExternalResourceFilePaths"))
        {
            aiMetadata resourceMetaData{};
            sceneMetaData->Get("AE_ExternalResourceFilePaths", resourceMetaData);
            activeScene.ExternalResourceFiles.reserve(resourceMetaData.mNumProperties);
            externalResourceIDMapping.reserve(resourceMetaData.mNumProperties);
            for (int i = 0; i < resourceMetaData.mNumProperties; i++)
            {
                SceneResourceID sceneResourceId = std::stoi(resourceMetaData.mKeys[i].C_Str());
                ASSERT(resourceMetaData.mValues[i].mType == aiMetadataType::AI_AISTRING, "Expected string in metadata!")
                aiString* resourceFilePath = (aiString*)resourceMetaData.mValues[i].mData;

                ASSERT(!externalResourceIDMapping.contains(sceneResourceId), "Expected scene resource to only appear once!")
                externalResourceIDMapping[sceneResourceId] = resourceFilePath->C_Str();
                activeScene.ExternalResourceFiles[resourceFilePath->C_Str()] = {.ReferenceCount = 0, .SceneResourceID = sceneResourceId};
            }
        }



        // Load entity data
        for (int i = 0; i < importedScene->mRootNode->mNumChildren; i++)
        {
            aiNode* node = importedScene->mRootNode->mChildren[i];
            Entity entity = sceneECS.CreateEntity(node->mName.C_Str());

            // Transform component is always present
            TransformComponent transformComponent{};
            aiMatrix4x4& transformMatrix = node->mTransformation;
            transformComponent.position = {transformMatrix.a1, transformMatrix.a2, transformMatrix.a3};
            transformComponent.scale = {transformMatrix.b1, transformMatrix.b2, transformMatrix.b3};
            transformComponent.rotation = {transformMatrix.c1, transformMatrix.c2, transformMatrix.c3};
            sceneECS.AddComponent(entity, transformComponent);

            if (node->mMetaData == nullptr) { continue; } // Check if the entity contains any metadata

            if (node->mMetaData->HasKey("Mesh_MeshData"))
            {
                // SceneResourceID meshDataResourceID;
                // node->mMetaData->Get("Mesh_MeshData", meshDataResourceID);
                // SceneResourceID materialResourceID;
                // node->mMetaData->Get("Mesh_Material", materialResourceID);
                aiString meshDataResourceID;
                node->mMetaData->Get("Mesh_MeshData", meshDataResourceID);
                aiString materialResourceID;
                node->mMetaData->Get("Mesh_Material", materialResourceID);
                MeshComponent meshComponent{};

                if (materialResourceID.C_Str() != "")
                {
                    // ASSERT(externalResourceIDMapping.contains(materialResourceID), "Expected scene to contain scene resource ID!")
                    // std::string materialFilePath = externalResourceIDMapping[materialResourceID];
                    // activeScene.ExternalResourceFiles[materialFilePath].ReferenceCount++;
                    meshComponent.Material = registry.CreateAsset<Material>(materialResourceID.C_Str());
                }
                else
                {
                    meshComponent.Material = nullptr;
                }

                if (meshDataResourceID.C_Str() != "")
                {
                    // ASSERT(externalResourceIDMapping.contains(meshDataResourceID), "Expected scene to contain scene resource ID!")
                    // std::string meshDataFilePath = externalResourceIDMapping[meshDataResourceID];
                    // activeScene.ExternalResourceFiles[meshDataFilePath].ReferenceCount++;
                    meshComponent.MeshData = registry.CreateAsset<Mesh>(meshDataResourceID.C_Str());
                }
                else
                {
                    meshComponent.MeshData = nullptr;
                }

                sceneECS.AddComponent(entity, meshComponent);
            }

            if (node->mMetaData->HasKey("Sprite_MeshData"))
            {
                // SceneResourceID meshDataResourceID;
                // node->mMetaData->Get("Sprite_MeshData", meshDataResourceID);
                // SceneResourceID materialResourceID;
                // node->mMetaData->Get("Sprite_Material", materialResourceID);
                aiString meshDataResourceID;
                node->mMetaData->Get("Sprite_MeshData", meshDataResourceID);
                aiString materialResourceID;
                node->mMetaData->Get("Sprite_Material", materialResourceID);
                SpriteComponent spriteComponent{};

                if (materialResourceID.C_Str() != "")
                {
                    // ASSERT(externalResourceIDMapping.contains(materialResourceID), "Expected scene to contain scene resource ID!")
                    // std::string materialFilePath = externalResourceIDMapping[materialResourceID];
                    // activeScene.ExternalResourceFiles[materialFilePath].ReferenceCount++;
                    spriteComponent.Material = registry.CreateAsset<Material>(materialResourceID.C_Str());
                }
                else
                {
                    spriteComponent.Material = nullptr;
                }

                if (meshDataResourceID.C_Str() != "")
                {
                    // ASSERT(externalResourceIDMapping.contains(meshDataResourceID), "Expected scene to contain scene resource ID!")
                    // std::string meshDataFilePath = externalResourceIDMapping[meshDataResourceID];
                    // activeScene.ExternalResourceFiles[meshDataFilePath].ReferenceCount++;
                    spriteComponent.MeshData = registry.CreateAsset<Mesh>(meshDataResourceID.C_Str());
                }
                else
                {
                    spriteComponent.MeshData = nullptr;
                }

                sceneECS.AddComponent(entity, spriteComponent);
            }

            if (node->mMetaData->HasKey("PointLightComponent_Intensity"))
            {
                double intensity;
                node->mMetaData->Get("PointLightComponent_Intensity", intensity);
                double x, y, z;
                node->mMetaData->Get("PointLightComponent_LightColor_X", x);
                node->mMetaData->Get("PointLightComponent_LightColor_Y", y);
                node->mMetaData->Get("PointLightComponent_LightColor_Z", z);

                PointLightComponent pointLightComponent{};
                pointLightComponent.Intensity = intensity;
                pointLightComponent.LightColor = Vec3(x, y, z);

                sceneECS.AddComponent(entity, pointLightComponent);
            }
        }

    }


    void SceneLoader::SerializeScene(Scene& scene, const std::filesystem::path& filePath)
    {
        ScopedPtr<aiScene> exportScene = CreateScopedPtr<aiScene>();
        exportScene->mName = filePath.stem().generic_string();
        ECS& ecs = scene.ECS;

        // Serialize external resource file paths in scene metadata
        aiMetadata resourceMetaData = aiMetadata();
        resourceMetaData.Alloc(scene.ExternalResourceFiles.size());
        for (const auto& [resourceFilePath, sceneResource] : scene.ExternalResourceFiles)
        {
            std::string sceneResourceID = std::to_string(sceneResource.SceneResourceID);
            resourceMetaData.Add(sceneResourceID, aiString(resourceFilePath));
        }
        exportScene->mMetaData = aiMetadata::Alloc(1);
        exportScene->mMetaData->Add("AE_ExternalResourceFilePaths", resourceMetaData);


        // Serialize ecs in scene node metadata
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        exportScene->mRootNode = new aiNode("Root Node"); // assimp will delete the root node when the aiScene goes out of scope
        for (Entity entity : ecs)
        {
            aiNode* node = new aiNode; // assimp will recursively delete all child nodes when the root node gets deleted
            node->mName = entity.GetDebugName().data();

            // Transformation Component always present
            TransformComponent transformComponent;
            ecs.GetComponent(entity, transformComponent);

            // Storing the position, scale, and rotation values inside the matrix instead of creating a matrix and then
            // converting it back to values on import
            aiMatrix4x4& transform = node->mTransformation;
            Vec3& position = transformComponent.position;
            Vec3& scale = transformComponent.scale;
            Vec3& rotation = transformComponent.rotation;
            transform.a1 = position.x; transform.a2 = position.y; transform.a3 = position.z;
            transform.b1 = scale.x; transform.b2 = scale.y; transform.b3 = scale.z;
            transform.c1 = rotation.x; transform.c2 = rotation.y; transform.c3 = rotation.z;


            if (ecs.HasComponent<MeshComponent>(entity))
            {
                MeshComponent meshComponent;
                ecs.GetComponent(entity, meshComponent);

                std::filesystem::path meshDataPath;
                std::filesystem::path materialPath;

                if (meshComponent.MeshData) { meshDataPath = registry.GetFilePathFromAssetID(meshComponent.MeshData->GetAssetID()); }
                else { meshDataPath = ""; }
                if (meshComponent.Material) { materialPath = registry.GetFilePathFromAssetID(meshComponent.Material->GetAssetID()); }
                else { materialPath = ""; }

                if (!node->mMetaData) { node->mMetaData = new aiMetadata; }
                node->mMetaData->Add("Mesh_MeshData", aiString(meshDataPath.generic_string())); // Could be empty strings -> ""
                node->mMetaData->Add("Mesh_Material", aiString(materialPath.generic_string()));
            }

            if (ecs.HasComponent<SpriteComponent>(entity))
            {
                SpriteComponent spriteComponent;
                ecs.GetComponent(entity, spriteComponent);

                std::filesystem::path meshDataPath;
                std::filesystem::path materialPath;

                if (spriteComponent.MeshData) { meshDataPath = registry.GetFilePathFromAssetID(spriteComponent.MeshData->GetAssetID()); }
                else { meshDataPath = ""; }
                if (spriteComponent.Material) { materialPath = registry.GetFilePathFromAssetID(spriteComponent.Material->GetAssetID()); }
                else { materialPath = ""; }

                if (!node->mMetaData) { node->mMetaData = new aiMetadata; }
                node->mMetaData->Add("Sprite_MeshData", aiString(meshDataPath.generic_string()));  // Could be empty strings -> ""
                node->mMetaData->Add("Sprite_Material", aiString(materialPath.generic_string()));
            }

            if (ecs.HasComponent<PointLightComponent>(entity))
            {
                PointLightComponent pointLightComponent;
                ecs.GetComponent(entity, pointLightComponent);

                if (!node->mMetaData) { node->mMetaData = new aiMetadata; }
                node->mMetaData->Add("PointLightComponent_Intensity", pointLightComponent.Intensity);
                node->mMetaData->Add("PointLightComponent_LightColor_X", pointLightComponent.LightColor.x);
                node->mMetaData->Add("PointLightComponent_LightColor_Y", pointLightComponent.LightColor.y);
                node->mMetaData->Add("PointLightComponent_LightColor_Z", pointLightComponent.LightColor.z);
            }

            exportScene->mRootNode->addChildren(1, &node);
        }


        // Scene file is in format of .gltf but when file extension of .aescene as not all gltf's can be imported as a scene
        // in the expected format
        Assimp::Exporter exporter;
        exporter.Export(exportScene.get(), "gltf2", filePath.string().c_str());
    }


    // TODO: Add a hashmap of already loaded aiMesh's as keys and their serialized file path as the value to prevent creating duplicates of the same mesh
    void SceneLoader::Helpers::LoadAndBreakObjectIntoMuiltipleObjects(const std::filesystem::path& sceneFilePath, bool shouldSerializeObjects)
    {
        std::filesystem::path sceneDir = sceneFilePath.parent_path();

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile( sceneFilePath.string(),
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType            |
        aiProcess_FlipUVs
        );

        // If the import failed, report it
        if (nullptr == scene)
        {
         WARN(importer.GetErrorString());
         return;
        }

        LOG("Loading Scene: " << scene->mName.C_Str());
        LOG("Scene has " << scene->mNumLights << " lights!")

        // Create directory for new files to go into for better organization
        std::filesystem::path destinationDir{};
        if (shouldSerializeObjects)
        {
            std::string destinationDirString = sceneDir.generic_string() + '/' + sceneFilePath.stem().generic_string() + '/';
            if (std::filesystem::exists(destinationDirString))
            {
                uint32 extension = 0;
                while (std::filesystem::exists(sceneDir.generic_string() + '/' + sceneFilePath.stem().generic_string() + "_" + std::to_string(extension) + '/'))
                {
                    extension++;
                }
                destinationDirString = sceneDir.generic_string() + '/' + sceneFilePath.stem().generic_string() + "_" + std::to_string(extension) + '/';
                LOG("Duplicate directory name found! Creating new directory at " << destinationDirString);
            }
            std::filesystem::create_directory(destinationDirString);
            destinationDir = destinationDirString;
        }



         AssetRegistry& assetRegistry = Engine::Get().GetAssetManager().GetRegistry();

         std::vector<Ref<Texture>> m_EmbeddedTextures;
         std::unordered_map<std::filesystem::path, Ref<Texture>> m_ExternalTextures;
         std::vector<Ref<Material>> m_Materials;
         std::vector<Ref<Mesh>> m_Meshes;

         // Load Embedded Textures
         if (scene->HasTextures())
         {
             for (int i = 0; i < scene->mNumTextures; i++)
             {
                 Ref<Texture> textureRef;
                 aiTexture* texture = scene->mTextures[i];
                 // TODO: Convert the format hint to a image format
                 textureRef = Texture::CreateTexture(texture->pcData, texture->mWidth, texture->mHeight, ImageFormat::R8G8B8A8_UNORM);
                 WARN("Loading an embedded texture! This probably won't work!")
                 m_EmbeddedTextures.push_back(textureRef);
             }
         }

         // Load Materials
         if (scene->HasMaterials())
         {

             for (int i = 0; i < scene->mNumMaterials; i++)
             {
                 aiMaterial* material = scene->mMaterials[i];
                 Ref<Material> materialRef;

                if (m_DefaultMaterialTextureConvention == TextureConvention::ORM_PACKED)
                {
                    materialRef = LoadORMPackedPBRMaterial(material, m_ExternalTextures, sceneDir);
                }
                else if (m_DefaultMaterialTextureConvention == TextureConvention::UNPACKED)
                {
                    materialRef = LoadPBRMaterial(material, m_ExternalTextures, sceneDir);
                }
                else
                {
                    ASTRAL_ERROR("Unsupported texture convention for material!")
                }


                 m_Materials.push_back(materialRef);

                 if (shouldSerializeObjects)
                 {
                     std::string filePathString = material->GetName().C_Str();
                     std::replace(filePathString.begin(), filePathString.end(), '\\', '/');
                     std::filesystem::path filePath = destinationDir / std::filesystem::path(filePathString);

                     filePath.replace_extension(".astmat");
                     if (std::filesystem::exists(filePath))
                     {
                         filePath.replace_extension("");
                         std::string uniqueFilePath = filePath.generic_string();
                         uint32 extension = 0;
                         while (std::filesystem::exists(uniqueFilePath + '_' + std::to_string(extension) + ".astmat"))
                         {
                             extension++;
                         }
                         filePath = uniqueFilePath + '_' + std::to_string(extension);
                         LOG("Duplicate material name found! Serializing to " << filePath);
                     }

                     MaterialLoader::SerializeMaterial(materialRef, filePath);
                     assetRegistry.RegisterAsset(materialRef, filePath);
                 }
             }
         }

         // Load Meshes
         if (scene->HasMeshes())
         {
             m_Meshes.reserve(scene->mNumMeshes);
             LOG("Number of meshes: " << scene->mNumMeshes)
             for (int i = 0; i < scene->mNumMeshes; i++)
             {
                 aiMesh* mesh = scene->mMeshes[i];
                 LOG("Mesh: " << mesh->mName.C_Str());
                 Ref<Mesh> meshRef = MeshLoader::LoadMesh(mesh);

                if (shouldSerializeObjects)
                {
                    std::string filePathString = mesh->mName.C_Str();
                    std::replace(filePathString.begin(), filePathString.end(), '\\', '/');
                    std::filesystem::path filePath = destinationDir / std::filesystem::path(filePathString);

                    filePath.replace_extension(".gltf");
                    if (std::filesystem::exists(filePath))
                    {
                        filePath.replace_extension("");
                        std::string uniqueFilePath = filePath.generic_string();
                        uint32 extension = 0;
                        while (std::filesystem::exists(uniqueFilePath + '_' + std::to_string(extension) + ".gltf"))
                        {
                         extension++;
                        }
                        filePath = uniqueFilePath + '_' + std::to_string(extension);
                        LOG("Duplicate mesh name found! Serializing to " << filePath);
                    }

                    MeshLoader::SerializeMesh(mesh, scene->mMaterials[mesh->mMaterialIndex], filePath);
                    assetRegistry.RegisterAsset(meshRef, filePath);
                }

                m_Meshes.push_back(meshRef);
            }
        }

        ECS& ecs = Engine::Get().GetSceneManager().GetECS();

        aiNode* rootNode = scene->mRootNode;
        std::vector<aiMatrix4x4> transforms;

        LOG("Total root child nodes: " << scene->mRootNode->mNumChildren)
        Helpers::ProcessSceneNode(scene, rootNode, m_Materials, m_Meshes);
    }


    void SceneLoader::Helpers::ProcessSceneNode(const aiScene* scene, const aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes)
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();


        Entity entity = ecs.CreateEntity(node->mName.C_Str());

        // Transform
        aiMatrix4x4 transformMatrix = node->mTransformation;

        const aiNode* currentParentNode = node;
        while (currentParentNode != scene->mRootNode)
        {
            currentParentNode = currentParentNode->mParent;
            transformMatrix = currentParentNode->mTransformation * transformMatrix;
        }


        TransformComponent transformComponent{};
        transformComponent.position = {transformMatrix.a4, transformMatrix.b4, transformMatrix.c4};
        Vec3 scaleX = {transformMatrix.a1, transformMatrix.b1, transformMatrix.c1};
        Vec3 scaleY = {transformMatrix.a2, transformMatrix.b2, transformMatrix.c2};
        Vec3 scaleZ = {transformMatrix.a3, transformMatrix.b3, transformMatrix.c3};
        transformComponent.scale = {scaleX.length(), scaleY.length(), scaleZ.length()};

        glm::mat3x3 rotationMatrix(
        transformMatrix.a1 / transformComponent.scale.x, transformMatrix.a2 / transformComponent.scale.y, transformMatrix.a3 / transformComponent.scale.z,
        transformMatrix.b1 / transformComponent.scale.x, transformMatrix.b2 / transformComponent.scale.y, transformMatrix.b3 / transformComponent.scale.z,
        transformMatrix.c1 / transformComponent.scale.x, transformMatrix.c2 / transformComponent.scale.y, transformMatrix.c3 / transformComponent.scale.z);

        glm::quat quat = {rotationMatrix};
        transformComponent.rotation = glm::eulerAngles(quat);

        ecs.AddComponent(entity, transformComponent);

        if (node->mNumMeshes > 0)
        {
            // Mesh
            MeshComponent meshComponent;
            int materialIndex = scene->mMeshes[node->mMeshes[0]]->mMaterialIndex;
            meshComponent.Material = materials[materialIndex];
            meshComponent.MeshData = meshes[node->mMeshes[0]];
            ecs.AddComponent(entity, meshComponent);

            // Create an additional entity per additional mesh in node
            for (int i = 1; i < node->mNumMeshes; i++)
            {
                Entity anotherEntity = ecs.CreateEntity(node->mName.C_Str());
                ecs.AddComponent(anotherEntity, transformComponent);
                MeshComponent meshComponent2;
                int materialIndex2 = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
                meshComponent2.Material = materials[materialIndex2];
                meshComponent2.MeshData = meshes[node->mMeshes[i]];
                ecs.AddComponent(anotherEntity, meshComponent2);
            }
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            ProcessSceneNode(scene, node->mChildren[i], materials, meshes);
        }
    }


    Ref<Material> SceneLoader::LoadPBRMaterial(aiMaterial* material, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::path& sceneDir)
    {
        Ref<Material> materialRef = CreateRef<Material>();

        LOG("Material: " << material->GetName().C_Str());

        Ref<Texture> baseColor;
        Ref<Texture> metallic;
        Ref<Texture> roughness;
        Ref<Texture> emission;
        Ref<Texture> normals;

        aiString baseColorFilePath;
        aiString metallicFilePath;
        aiString roughnessFilePath;
        aiString emissionFilePath;
        aiString normalsFilePath;


        material->GetTexture(aiTextureType_BASE_COLOR, 0, &baseColorFilePath);
        material->GetTexture(aiTextureType_METALNESS, 0, &metallicFilePath);
        material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughnessFilePath);
        material->GetTexture(aiTextureType_EMISSIVE, 0, &emissionFilePath);
        material->GetTexture(aiTextureType_NORMALS, 0, &normalsFilePath);


        // Base Color

        if (baseColorFilePath.length != 0)
        {
            baseColor = GetTexture(baseColorFilePath, externalTextures, sceneDir);
        }
        else
        {
            LOG("No base color. Trying diffuse...")
            material->GetTexture(aiTextureType_DIFFUSE, 0, &baseColorFilePath);

            if (baseColorFilePath.length == 0)
            {
                LOG("Missing base color!")
                baseColor = m_DefaultMaterialBaseColor;
            }
            else
            {
                LOG("Diffuse found! Using diffuse instead of base color map!")
                baseColor = GetTexture(baseColorFilePath, externalTextures, sceneDir);
                if (!baseColor) { baseColor = m_DefaultMaterialBaseColor;  }
            }
        }


        //  Metallic

        if (metallicFilePath.length != 0)
        {
            metallic = GetTexture(metallicFilePath, externalTextures, sceneDir);
            if (!metallic) { metallic = m_DefaultMaterialMetallic; }
        }
        else
        {
            LOG("No metallicness map! Using default!")
            metallic = m_DefaultMaterialMetallic;
        }


        //  Roughness

        if (roughnessFilePath.length != 0)
        {
            roughness = GetTexture(roughnessFilePath, externalTextures, sceneDir);
            if (!roughness) { roughness = m_DefaultMaterialRoughness; }
        }
        else
        {
            LOG("No roughness map! Using default!")
            roughness = m_DefaultMaterialRoughness;
        }


        //  Emission

        if (emissionFilePath.length != 0)
        {
            emission = GetTexture(emissionFilePath, externalTextures, sceneDir);
            if (!emission) { emission = m_DefaultMaterialEmission; }
        }
        else
        {
            LOG("No emission map! Using default!")
            emission = m_DefaultMaterialEmission;
        }


        //  Normals

        if (normalsFilePath.length != 0)
        {
            normals = GetTexture(normalsFilePath, externalTextures, sceneDir);
            materialRef->HasNormalMap = true;
        }
        else
        {
            LOG("No normal map! Using vertex normals instead!")
            normals = m_DefaultMaterialNormals;
            materialRef->HasNormalMap = false;
        }


        AssetRegistry& assetRegistry = Engine::Get().GetAssetManager().GetRegistry();

        materialRef->ShaderModel = ShaderModel::PBR;
        materialRef->TextureConvention = TextureConvention::UNPACKED;
        materialRef->FragmentShader = assetRegistry.GetAsset<Shader>("Shaders/brdf.frag");

        materialRef->Textures.push_back(baseColor);
        materialRef->Textures.push_back(metallic);
        materialRef->Textures.push_back(roughness);
        materialRef->Textures.push_back(emission);
        materialRef->Textures.push_back(normals);

        materialRef->DescriptorSet = DescriptorSet::CreateDescriptorSet();
        materialRef->DescriptorSet->BeginBuildingSet();
        materialRef->DescriptorSet->AddDescriptorImageSampler(baseColor, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(metallic, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(roughness, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(emission, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(normals, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->EndBuildingSet();

        return materialRef;
    }


    Ref<Material> SceneLoader::LoadORMPackedPBRMaterial(aiMaterial* material, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::path& sceneDir)
    {
        AssetRegistry& assetRegistry = Engine::Get().GetAssetManager().GetRegistry();
        Ref<Material> materialRef = CreateRef<Material>();

        LOG("Material: " << material->GetName().C_Str());

        Ref<Texture> baseColor;
        Ref<Texture> normals;
        Ref<Texture> aoRoughnessMetallicPacked;
        Ref<Texture> emission;

        aiString baseColorFilePath;
        aiString normalsFilePath;
        aiString ormPackedFilePath;
        aiString emissionFilePath;


        // Loading base color
        material->GetTexture(aiTextureType_BASE_COLOR, 0, &baseColorFilePath);
        material->GetTexture(aiTextureType_NORMALS, 0, &normalsFilePath);
        material->GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &ormPackedFilePath);
        material->GetTexture(aiTextureType_EMISSIVE, 0, &emissionFilePath);


        // Base Color

        if (baseColorFilePath.length != 0)
        {
            baseColor = GetTexture(baseColorFilePath, externalTextures, sceneDir);
        }
        else
        {
            LOG("No base color. Trying diffuse...")
            material->GetTexture(aiTextureType_DIFFUSE, 0, &baseColorFilePath);

            if (baseColorFilePath.length == 0)
            {
                LOG("Missing base color!")
                baseColor = m_DefaultMaterialBaseColor;
            }
            else
            {
                LOG("Diffuse found! Using diffuse instead of base color map!")
                baseColor = GetTexture(baseColorFilePath, externalTextures, sceneDir);
                if (!baseColor) { baseColor = m_DefaultMaterialBaseColor;  }
            }
        }


        //  Normals

        if (normalsFilePath.length != 0)
        {
            normals = GetTexture(normalsFilePath, externalTextures, sceneDir);
            materialRef->HasNormalMap = true;
        }
        else
        {
            LOG("No normal map! Using vertex normals instead!")
            normals = m_DefaultMaterialNormals;
            materialRef->HasNormalMap = false;
        }


        // AO-Metallic-Roughness Packed

        if (ormPackedFilePath.length != 0)
        {
            aoRoughnessMetallicPacked = GetTexture(ormPackedFilePath, externalTextures, sceneDir);
            if (!aoRoughnessMetallicPacked) { aoRoughnessMetallicPacked = m_DefaultMaterialRoughness; }
        }
        else
        {
            LOG("No metallicness map! Using default!")
            aoRoughnessMetallicPacked = m_DefaultMaterialRoughness;
        }


        //  Emission

        if (emissionFilePath.length != 0)
        {
            emission = GetTexture(emissionFilePath, externalTextures, sceneDir);
            if (!emission) { emission = m_DefaultMaterialEmission; }
        }
        else
        {
            LOG("No emission map! Using default!")
            emission = m_DefaultMaterialEmission;
        }


        materialRef->ShaderModel = ShaderModel::PBR;
        materialRef->TextureConvention = TextureConvention::ORM_PACKED;
        materialRef->FragmentShader = assetRegistry.GetAsset<Shader>("Shaders/brdf.frag");

        materialRef->Textures.push_back(baseColor);
        materialRef->Textures.push_back(aoRoughnessMetallicPacked);
        materialRef->Textures.push_back(emission);
        materialRef->Textures.push_back(normals);

        materialRef->DescriptorSet = DescriptorSet::CreateDescriptorSet();
        materialRef->DescriptorSet->BeginBuildingSet();
        materialRef->DescriptorSet->AddDescriptorImageSampler(baseColor, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(aoRoughnessMetallicPacked, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(emission, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->AddDescriptorImageSampler(normals, ShaderStage::FRAGMENT);
        materialRef->DescriptorSet->EndBuildingSet();

        return materialRef;
    }


    Ref<Texture> SceneLoader::GetTexture(aiString& filePath,
                                         std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::path& sceneDir)
    {
        AssetRegistry& assetRegistry = Engine::Get().GetAssetManager().GetRegistry();
        Ref<Texture> textureRef;
        if (filePath.data[0] == '*')
        {
            // Embedded Texture
            std::string embeddedFileID = filePath.C_Str();
            int embeddedTextureIndex = std::stoi(embeddedFileID.substr(1));
            // textureRef = m_EmbeddedTextures[embeddedTextureIndex];
            ASTRAL_ERROR("Embedded Textures are disabled! (For now)");
        }
        else
        {
            // External Texture
            std::string filePathString = filePath.C_Str();
            std::replace(filePathString.begin(), filePathString.end(), '\\', '/');
            std::filesystem::path fullFilePath = sceneDir / std::filesystem::path(filePathString);

            if (externalTextures.contains(fullFilePath))
            {
                textureRef = externalTextures[fullFilePath];
            }
            else
            {
                textureRef = Texture::CreateTexture(fullFilePath.generic_string());
                assetRegistry.RegisterAsset(textureRef, fullFilePath.generic_string());
                externalTextures[fullFilePath] = textureRef;
            }
        }

        return textureRef;
    }

}







