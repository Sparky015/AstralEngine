/**
* @file AssetRegistry.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include <unordered_map>
#include <filesystem>

#include "Asset.h"
#include "AssetRegistryStats.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral {

    using AssetID = size_t;
    static constexpr AssetID NullAssetID = 0;

    class AssetRegistry
    {
     public:

        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        AssetID CreateAsset(const std::filesystem::path& filePath);

        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        AssetType* GetAsset(AssetID assetID);

        void UnloadAsset(AssetID assetID);
        void UnloadAsset(const std::filesystem::path& filePath);

        Asset* LoadAsset(AssetType assetType, const std::filesystem::path& filePath);

        void SetAssetDirectoryPath(const std::filesystem::path& filePath) { m_AssetDirectoryPath = filePath; }
        AssetID GetAssetIDFromFilePath(const std::filesystem::path& filePath);
        [[nodiscard]] const AssetRegistryStats& GetAssetRegistryStats() const { return m_RegistryStats; }

    private:

        AssetID AssignNextAvailableAssetID();

        AssetID m_NextAvailableAssetID{1};
        std::filesystem::path m_AssetDirectoryPath;

        std::unordered_map<std::filesystem::path, AssetID> m_FilePathToAssetID;
        std::unordered_map<AssetID, std::filesystem::path> m_AssetIDToFilePath;
        std::unordered_map<AssetID, Asset*> m_AssetIDToAsset;

        AssetRegistryStats m_RegistryStats{};
    };

}

#include "AssetRegistry_Impl.h"
