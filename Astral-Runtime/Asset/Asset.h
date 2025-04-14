/**
* @file Asset.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include <filesystem>

#include "Core/FixedIntegerTypes.h"
#include "AssetErrorCodes.h"

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

}
