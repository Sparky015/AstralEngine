/**
* @file EnvironmentMap.h
* @author Andrew Fagan
* @date 8/27/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    /**
     * @struct EnvironmentMap
     * @brief Defines a set of environment maps used for environment IBL lighting
     * @details Contains the original environment map, irradiance map, and prefiltered environment map for environment image-based lighting
     */
    struct EnvironmentMap : public Asset
    {
        TextureHandle Environment;
        TextureHandle Irradiance;
        TextureHandle PrefilteredEnvironment;

        /**
         * @brief Gets the type of the asset (AssetType::EnvironmentMap)
         * @return The type of asset (AssetType::EnvironmentMap)
         */
        AssetType GetAssetType() override { return GetStaticAssetType(); }

        /**
         * @brief Gets the type of the asset (AssetType::EnvironmentMap)
         * @return The type of asset (AssetType::EnvironmentMap)
         */
        static AssetType GetStaticAssetType() { return AssetType::EnvironmentMap; }
    };

}
