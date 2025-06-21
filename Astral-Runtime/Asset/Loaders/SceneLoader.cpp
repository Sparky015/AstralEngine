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

#include "MeshLoader.h"
#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Core/SmartPointers.h"
#include "ECS/ECSManager.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral::SceneLoader {

    void LoadSceneAssets(const std::filesystem::path& filePath)
    {
        std::string pFile = filePath.string();
        std::filesystem::path sceneDir = filePath.parent_path();


        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile( pFile,
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

                textureRef = Texture::CreateTexture(texture->pcData, texture->mWidth, texture->mHeight);
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
                        m_Materials.push_back(nullptr);
                        continue;
                    }
                    else
                    {
                        LOG("Base color found!")
                    }
                }

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
                        textureRef = Texture::CreateTexture(fullFilePath.string());
                        m_ExternalTextures[fullFilePath] = textureRef;
                    }
                }

                materialRef->DescriptorSet = DescriptorSet::CreateDescriptorSet();
                materialRef->DescriptorSet->BeginBuildingSet();
                materialRef->DescriptorSet->AddDescriptorImageSampler(textureRef, ShaderStage::FRAGMENT);
                materialRef->DescriptorSet->EndBuildingSet();

                materialRef->VertexShader = assetRegistry.GetAsset<Shader>("Shaders/Mesh_Position_UV.vert");
                materialRef->FragmentShader = assetRegistry.GetAsset<Shader>("Shaders/Sample_Image.frag");

                m_Materials.push_back(materialRef);
            }
        }

        // Load Meshes
        if (scene->HasMeshes())
        {
            for (int i = 0; i < scene->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[i];
                Ref<Mesh> meshRef = MeshLoader::LoadMesh(mesh);
                m_Meshes.push_back(meshRef);
            }
        }

        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        aiNode* rootNode = scene->mRootNode;
        std::vector<aiMatrix4x4> transforms;

        Helpers::ProcessSceneNode(scene, rootNode, m_Materials, m_Meshes);

        // for (int i = 0; i < rootNode->mNumMeshes; i++)
        // {
        //     Entity entity = ecs.CreateEntity(rootNode->mName.C_Str());
        //     aiMatrix4x4 transformMatrix = rootNode->mTransformation;
        //     TransformComponent transformComponent{};
        //     transformComponent.position = {transformMatrix.d1, transformMatrix.d2, transformMatrix.d3};
        //     Vec3 scaleX = {transformMatrix.a1, transformMatrix.a2, transformMatrix.a3};
        //     Vec3 scaleY = {transformMatrix.b1, transformMatrix.b2, transformMatrix.b3};
        //     Vec3 scaleZ = {transformMatrix.c1, transformMatrix.c2, transformMatrix.c3};
        //     transformComponent.scale = {scaleX.length(), scaleY.length(), scaleZ.length()};
        //     ecs.AddComponent(entity, transformComponent);
        //
        //     MeshComponent meshComponent;
        //     meshComponent.Material = m_Materials[scene->mMeshes[rootNode->mMeshes[i]]->mMaterialIndex];
        //     meshComponent.MeshData = m_Meshes[rootNode->mMeshes[i]];
        //     ecs.AddComponent(entity, meshComponent);
        // }


    }


    void Helpers::ProcessSceneNode(const aiScene* scene, const aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes)
    {
        ECS& ecs = Engine::Get().GetECSManager().GetECS();
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
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            ProcessSceneNode(scene, node->mChildren[i], materials, meshes);
        }
    }

}
