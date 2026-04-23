/**
* @file AssetRegistryStats.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once


#include "Core/FixedIntegerTypes.h"
#include "Asset.h"
#include <unordered_map>

namespace Astral {

    struct AssetRegistryStats
    {
        uint32 NumberOfLoadedAssets;
        uint64 NumberOfLoadsMade;
        uint64 NumberOfUnloadsMade;
        std::unordered_map<AssetType, uint32> LoadedAssetsByType;
    };

}