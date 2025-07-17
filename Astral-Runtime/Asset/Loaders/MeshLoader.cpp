/**
* @file MeshLoader.cpp
* @author Andrew Fagan
* @date 6/16/25
*/

#include "MeshLoader.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Asset/AssetManager.h"
#include "Core/Engine.h"

#include <assimp/Importer.hpp>
#include "assimp/Exporter.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp/SceneCombiner.h"


namespace Astral::MeshLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MeshLoader::LoadAsset")

        std::string pFile = filePath.string();

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile( pFile,
        aiProcess_GenNormals         |
          aiProcess_CalcTangentSpace       |
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices  |
          aiProcess_FlipWindingOrder       |
          aiProcess_SortByPType
          );

        // If the import failed, report it
        if (nullptr == scene)
        {
            WARN(importer.GetErrorString());
            return nullptr;
        }


        if (!scene->HasMeshes()) { return nullptr; }

        aiMesh* mesh = scene->mMeshes[0];
        if (scene->mNumMeshes > 1) {  WARN("Loading 3D object file with more than 1 mesh! Ignoring " << scene->mNumMeshes << " other meshes!") }

        return LoadMesh(mesh);
    }


    Ref<Mesh> LoadMesh(aiMesh* mesh)
    {
        std::vector<float> vertexData{};

        VertexBufferLayout bufferLayout = {
            {Float3, "Position"}
        };

        if (mesh->HasNormals())
        {
            bufferLayout.AddAttribute({Float3, "Normals"});
        }

        if (mesh->HasTangentsAndBitangents())
        {
            bufferLayout.AddAttribute({Float3, "Tangents"});
            bufferLayout.AddAttribute({Float3, "Bitangents"});
        }

        if (mesh->HasTextureCoords(0))
        {
            bufferLayout.AddAttribute({Float2, "UVs"});
        }

        vertexData.reserve(bufferLayout.GetStride() * mesh->mNumVertices);

        for (uint32 i = 0; i < mesh->mNumVertices; i++)
        {
            // Position
            for (uint32 j = 0; j < 3; j++)
            {
                vertexData.push_back(mesh->mVertices[i][j]);
            }

            if (mesh->HasNormals())
            {
                for (uint32 j = 0; j < 3; j++)
                {
                    vertexData.push_back(mesh->mNormals[i][j]);
                }
            }

            if (mesh->HasTangentsAndBitangents())
            {
                for (uint32 j = 0; j < 3; j++)
                {
                    vertexData.push_back(mesh->mTangents[i][j]);
                }

                for (uint32 j = 0; j < 3; j++)
                {
                    vertexData.push_back(mesh->mBitangents[i][j]);
                }
            }

            if (mesh->HasTextureCoords(0))
            {
                for (uint32 j = 0; j < 2; j++)
                {
                    vertexData.push_back(mesh->mTextureCoords[0][i][j]);
                }
            }
        }


        std::vector<uint32> indiceData{};
        indiceData.reserve(mesh->mNumFaces * 3);

        for (uint32 i = 0; i < mesh->mNumFaces; i++)
        {
            for (uint32 j = 0; j < 3; j++)
            {
                indiceData.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }


        IndexBufferHandle indexBuffer = IndexBuffer::CreateIndexBuffer(indiceData.data(), indiceData.size());
        VertexBufferHandle vertexBuffer = VertexBuffer::CreateVertexBuffer(vertexData.data(), vertexData.size() * sizeof(float), bufferLayout);

        AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        Ref<Shader> vertexShader;

        if (mesh->HasNormals() && mesh->HasTextureCoords(0))
        {
            vertexShader = registry.CreateAsset<Shader>("Shaders/Mesh_Position_Normals_UV.vert");
        }
        else if (mesh->HasTextureCoords(0))
        {
            vertexShader = registry.CreateAsset<Shader>("Shaders/Mesh_Position_UV.vert");
        }
        else
        {
            WARN("Unknown mesh data combination needed to pick vertex shader!")
            return nullptr;
        }

        Mesh meshInstance{};
        meshInstance.VertexBuffer = vertexBuffer;
        meshInstance.IndexBuffer = indexBuffer;
        meshInstance.VertexShader = vertexShader;


        return CreateRef<Mesh>(meshInstance);
    }


    void SerializeMesh(aiMesh* mesh, aiMaterial* material, std::filesystem::path& outFilePath)
    {
        ScopedPtr<aiScene> exportScene = CreateScopedPtr<aiScene>();
        outFilePath.replace_extension(".gltf");

        aiMesh** meshes = new aiMesh*[1];
        aiMaterial** materials = new aiMaterial*[1];
        Assimp::SceneCombiner::Copy(meshes, mesh);
        Assimp::SceneCombiner::Copy(materials, material);
        aiMesh* copiedMesh = meshes[0];

        exportScene->mName = copiedMesh->mName;
        exportScene->mRootNode = new aiNode(copiedMesh->mName.C_Str()); // assimp will delete the root node when the aiScene goes out of scope
        exportScene->mNumMeshes = 1;
        exportScene->mMeshes = meshes;
        exportScene->mNumMaterials = 1;
        exportScene->mMaterials = materials;
        copiedMesh->mMaterialIndex = 0;
        exportScene->mRootNode->mNumMeshes = 1;
        unsigned int* meshIndex = new unsigned int[1];
        *meshIndex = 0;
        exportScene->mRootNode->mMeshes = meshIndex;

        Assimp::Exporter exporter;
        exporter.Export(exportScene.get(), "gltf2", outFilePath.string().c_str(), aiProcess_CalcTangentSpace       |
                                                                                                                      aiProcess_Triangulate            |
                                                                                                                      aiProcess_JoinIdenticalVertices  |
                                                                                                                      aiProcess_SortByPType);
    }

}
