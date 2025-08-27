/**
* @file EnvironmentMap.h
* @author Andrew Fagan
* @date 8/27/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    struct EnvironmentMap : public Asset
    {
        TextureHandle Environment;
        TextureHandle Irradiance;

        AssetType GetAssetType() override { return GetStaticAssetType(); };
        static AssetType GetStaticAssetType() { return AssetType::EnvironmentMap; }
    };

}
