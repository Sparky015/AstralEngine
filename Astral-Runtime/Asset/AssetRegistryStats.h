/**
* @file AssetRegistryStats.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    struct AssetRegistryStats
    {
        uint32 NumberOfLoadedAssets;
        uint64 NumberOfLoadsMade;
        uint64 NumberOfUnloadsMade;
    };

}