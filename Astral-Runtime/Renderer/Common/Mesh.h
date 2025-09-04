/**
* @file Mesh.h
* @author Andrew Fagan
* @date 5/24/2025
*/


#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

namespace Astral {

    struct BoundingSphere
    {
        float Radius;
        Vec3 Center;
    };

    struct Mesh : Asset
    {
        VertexBufferHandle VertexBuffer;
        IndexBufferHandle IndexBuffer;
        Ref<Shader> VertexShader;
        BoundingSphere BoundingSphere;

        AssetType GetAssetType() override { return Mesh::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Mesh; }
    };

}
