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

    /**
     * @struct BoundingSphere
     * @brief Defines a bounding sphere with a point and radius
     */
    struct BoundingSphere
    {
        float Radius;
        Vec3 Center;
    };

    /**
     * @struct Mesh
     * @brief Defines a Mesh used for rendering
     * @details Stores buffers for mesh data as well as a bounding sphere for culling and the vertex shader that matches the vertex data layout
     */
    struct Mesh : Asset
    {
        VertexBufferHandle VertexBuffer;
        IndexBufferHandle IndexBuffer;
        Ref<Shader> VertexShader;
        BoundingSphere BoundingSphere;

        /**
         * @brief Gets the type of the asset (AssetType::Mesh)
         * @return The type of asset (AssetType::Mesh)
         */
        AssetType GetAssetType() override { return Mesh::GetStaticAssetType(); }

        /**
         * @brief Gets the type of the asset (AssetType::Mesh)
         * @return The type of asset (AssetType::Mesh)
         */
        static AssetType GetStaticAssetType() { return AssetType::Mesh; }
    };

}
