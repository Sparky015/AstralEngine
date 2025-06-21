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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Astral::MeshLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("MeshLoader::LoadAsset")

        std::string pFile = filePath.string();

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
            return nullptr;
        }


        if (!scene->HasMeshes()) { return nullptr; }

        aiMesh* mesh = scene->mMeshes[0];

        return LoadMesh(mesh);
    }


    Ref<Mesh> LoadMesh(aiMesh* mesh)
    {
        std::vector<float> vertexData{};

        VertexBufferLayout bufferLayout = {
            {Float3, "Position"}
        };

        // if (mesh->HasNormals())
        // {
        //     bufferLayout.AddAttribute({Float3, "Normals"});
        // }
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

            // if (mesh->HasNormals())
            // {
            //     for (uint32 j = 0; j < 3; j++)
            //     {
            //         vertexData.push_back(mesh->mNormals[i][j]);
            //     }
            // }

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

        Mesh meshInstance{};
        meshInstance.VertexBuffer = vertexBuffer;
        meshInstance.IndexBuffer = indexBuffer;

        return CreateRef<Mesh>(meshInstance);
    }

}
