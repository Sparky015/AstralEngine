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
#include "Core/SmartPointers.h"

namespace Astral {

    struct Scene; // Forward Declared

    class AssetRegistry
    {
     public:

        AssetRegistry() = default;
        ~AssetRegistry();

        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> CreateAsset(const std::filesystem::path& filePath);

        void LoadScene(const std::filesystem::path& filePath);
        void SerializeScene(Scene& scene, const std::filesystem::path& filePath);

        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> GetAsset(AssetID assetID);

        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> GetAsset(std::string_view filePath);

        void UnloadAsset(AssetID assetID);
        void UnloadAsset(const std::filesystem::path& filePath);

        Ref<Asset> LoadAsset(AssetType assetType, const std::filesystem::path& filePath);

        void SetAssetDirectoryPath(const std::filesystem::path& filePath) { m_AssetDirectoryPath = filePath; }
        AssetID GetAssetIDFromFilePath(const std::filesystem::path& filePath);
        std::filesystem::path GetFilePathFromAssetID(AssetID assetID);
        [[nodiscard]] const AssetRegistryStats& GetAssetRegistryStats() const { return m_RegistryStats; }
        const std::filesystem::path& GetAssetDirectoryPath() { return m_AssetDirectoryPath; }

    private:

        AssetID AssignNextAvailableAssetID();

        AssetID m_NextAvailableAssetID{0};
        std::filesystem::path m_AssetDirectoryPath;

        std::unordered_map<std::filesystem::path, AssetID> m_FilePathToAssetID;
        std::unordered_map<AssetID, std::filesystem::path> m_AssetIDToFilePath;
        std::unordered_map<AssetID, Ref<Asset>> m_AssetIDToAsset;

        AssetRegistryStats m_RegistryStats{};
    };

}

#include "AssetRegistry_Impl.h"
