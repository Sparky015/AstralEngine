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

    struct Mesh : Asset
    {
        VertexBufferHandle VertexBuffer;
        IndexBufferHandle IndexBuffer;
        Ref<Shader> VertexShader;

        AssetType GetAssetType() override { return Mesh::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Mesh; }
    };

}
