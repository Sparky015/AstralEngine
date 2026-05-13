/**
* @file CubeLUT.h
* @author Andrew Fagan
* @date 9/4/25
*/

#pragma once

#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    /**
     * @struct CubeLUT
     * @brief Defines a 3D look-up table containing the shaper and a 3D texture used for look-ups
     */
    struct CubeLUT : public Asset
    {
        TextureHandle Shaper1D;
        TextureHandle LUT3D;
        Vec2 ShaperInputRange;

        /**
         * @brief Gets the type of the asset (AssetType::CubeLUT)
         * @return The type of asset (AssetType::CubeLUT)
         */
        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }

        /**
         * @brief Gets the type of the asset (AssetType::CubeLUT)
         * @return The type of asset (AssetType::CubeLUT)
         */
        static AssetType GetStaticAssetType() { return AssetType::CubeLUT; }
    };

}
