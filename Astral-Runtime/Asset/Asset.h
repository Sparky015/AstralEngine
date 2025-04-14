/**
* @file Asset.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "AssetErrorCodes.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    enum class AssetType : uint8
    {
        Texture
    };

    class Asset
    {
    public:
        virtual ~Asset() = default;

        virtual AssetType GetAssetType() = 0; // For debug and editor purposes
    };

    inline const char* AssetTypeToString(AssetType assetType)
    {
        switch (assetType)
        {
            case AssetType::Texture: return "Texture";
            default: ASTRAL_ERROR("Invalid asset type value given!");
        }
    }

}
