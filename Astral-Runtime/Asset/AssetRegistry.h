/**
* @file AssetRegistry.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include <unordered_map>
#include <filesystem>

namespace Astral {

    using AssetID = size_t;

    class AssetRegistry
    {
     public:

         static AssetRegistry& Get();

    private:

        std::unordered_map<std::filesystem::path, AssetID> m_FilePathToAssetID;
        std::unordered_map<AssetID, int> m_AssetIDToAsset;

    };

}