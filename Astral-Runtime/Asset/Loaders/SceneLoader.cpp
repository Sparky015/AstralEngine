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

namespace Astral::SceneLoader {

    void LoadSceneAssets(const std::filesystem::path& sceneFilePath)
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


    void SerializeScene(Scene& scene, const std::filesystem::path& filePath)
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
    void Helpers::LoadAndBreakObjectIntoMuiltipleObjects(const std::filesystem::path& sceneFilePath, bool shouldSerializeObjects)
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
                 Ref<Material> materialRef = CreateRef<Material>();

                 // for (int j = 0; j < material->mNumProperties; j++)
                 // {
                 //     aiMaterialProperty* materialProperty = material->mProperties[j];
                 // }

                 // Load just the diffuse map for now
                 aiString textureFilePath;
                 material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);


                 if (textureFilePath.length == 0)
                 {
                     LOG("No Diffuse. Trying base color...")
                     material->GetTexture(aiTextureType_BASE_COLOR, 0, &textureFilePath);

                     if (textureFilePath.length == 0)
                     {
                         LOG("No base color. Discarding material!")
                         m_Materials.push_back(assetRegistry.GetAsset<Material>("Materials/MissingMaterial.astmat"));
                     }
                     else
                     {
                         LOG("Base color found!")
                     }
                 }
                 else
                 {
                     Ref<Texture> textureRef;

                     if (textureFilePath.data[0] == '*')
                     {
                         // Embedded Texture
                         std::string embeddedFileID = textureFilePath.C_Str();
                         int embeddedTextureIndex = std::stoi(embeddedFileID.substr(1));
                         textureRef = m_EmbeddedTextures[embeddedTextureIndex];
                     }
                     else
                     {
                         // External Texture
                         std::filesystem::path fullFilePath = sceneDir / std::filesystem::path(textureFilePath.C_Str());
                         if (m_ExternalTextures.contains(fullFilePath))
                         {
                             textureRef = m_ExternalTextures[fullFilePath];
                         }
                         else
                         {
                             textureRef = Texture::CreateTexture(fullFilePath.generic_string());
                             assetRegistry.RegisterAsset(textureRef, fullFilePath.generic_string());
                             m_ExternalTextures[fullFilePath] = textureRef;
                         }
                     }

                     materialRef->DescriptorSet = DescriptorSet::CreateDescriptorSet();
                     materialRef->DescriptorSet->BeginBuildingSet();
                     materialRef->DescriptorSet->AddDescriptorImageSampler(textureRef, ShaderStage::FRAGMENT);
                     materialRef->DescriptorSet->EndBuildingSet();
                     materialRef->FragmentShader = assetRegistry.GetAsset<Shader>("Shaders/Sample_Image.frag");
                     materialRef->Textures.push_back(textureRef);

                     LOG("Material: " << material->GetName().C_Str());

                     if (shouldSerializeObjects)
                     {
                         std::filesystem::path filePath = destinationDir / std::filesystem::path(material->GetName().C_Str());

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

                     m_Materials.push_back(materialRef);
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
                    std::filesystem::path filePath = destinationDir / std::filesystem::path(mesh->mName.C_Str());
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


    void Helpers::ProcessSceneNode(const aiScene* scene, const aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes)
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

}







