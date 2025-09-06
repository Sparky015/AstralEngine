/**
* @file CubeLUT.h
* @author Andrew Fagan
* @date 9/4/25
*/

#pragma once

#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    struct CubeLUT : public Asset
    {
        TextureHandle Shaper1D;
        TextureHandle LUT3D;
        Vec2 ShaperInputRange;

        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::CubeLUT; }
    };

}
