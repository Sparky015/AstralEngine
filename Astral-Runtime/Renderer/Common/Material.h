/**
* @file Material.h
* @author Andrew Fagan
* @date 5/24/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Asset/Asset.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

    struct Material : public Asset
    {
        AssetID VertexShaderID;
        AssetID PixelShaderID;
        AssetID TextureID;

        AssetType GetAssetType() override { return GetStaticAssetType(); };
        static AssetType GetStaticAssetType() { return AssetType::Material; }
    };

}
