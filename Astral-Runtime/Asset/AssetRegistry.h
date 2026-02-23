/**
* @file AssetRegistry.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include <unordered_map>
#include <filesystem>
#include <future>

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

        /**
         * @brief Initializes default asset and engine asset directories
         */
        void Init();

        /**
         * @brief Cleans up static resources in the Asset Registry
         */
        void Shutdown();

        /**
         * @brief Loads an asset from disk and registers it into the Asset Registry only if it is not already loaded and registered.
         *        If it is already registered, then the currently loaded asset is returned.
         * @param filePath The file path of the asset to load
         * @return The requested asset
         */
        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> CreateAsset(const std::filesystem::path& filePath);

        /**
         * @brief Takes an already loaded asset and registers it into the Asset Registry with the given file path.
         * @param alreadyLoadedAsset The asset being registered
         * @param filePath The file path to associate the asset with
         * @note This does nothing if the asset is already registered
         */
        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        void RegisterAsset(Ref<AssetType> alreadyLoadedAsset, const std::filesystem::path& filePath);

        /**
         * @brief Takes an already loaded asset and registers it into the Asset Registry with a unique ID (instead of a file path).
         * @param alreadyLoadedAsset The asset being registered
         * @param uniqueIdentifier A unique ID to associate the asset with
         * @note The purpose of this function is to be able to generate assets at runtime that don't need to be stored to disk but should be cached.
         *       This does nothing if the asset is already registered.
         */
        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        void RegisterRuntimeAsset(Ref<AssetType> alreadyLoadedAsset, const std::string& uniqueIdentifier);

        /**
         * @brief
         */
        void LoadScene(const std::filesystem::path& filePath);

        /**
         * @brief
         */
        void SerializeScene(Scene& scene, const std::filesystem::path& filePath);

        /**
         * @brief Retrieves an asset from the Asset Registry
         * @param assetID The ID of the asset to retrieve
         * @return The asset being requested or nullptr if it does not exist in the registry
         */
        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> GetAsset(AssetID assetID);

        /**
         * @brief Retrieves an asset from the Asset Registry
         * @param filePath The filePath (or unique runtime ID) of the asset to retrieve
         * @return The asset being requested or nullptr if it does not exist in the registry
         */
        template <typename AssetType>
            requires std::is_base_of_v<Asset, AssetType>
        Ref<AssetType> GetAsset(std::string_view filePath);

        /**
         * @brief Unregisters an asset from the Asset Registry
         * @param assetID The asset ID of the asset to unload
         * @note This does not unload the asset right away. This only removes the reference stored in the Asset Registry
         *       This does nothing if the asset is not in the Asset Registry.
         */
        void UnloadAsset(AssetID assetID);

        /**
         * @brief Unregisters an asset from the Asset Registry
         * @param filePath The file path of the asset to unload
         * @note This does not unload the asset right away. This only removes the reference stored in the Asset Registry.
         *       This does nothing if the asset is not in the Asset Registry.
         */
        void UnloadAsset(const std::filesystem::path& filePath);

        /**
         * @brief Loads an asset from disk but does not register it
         * @param assetType The type of the asset to load
         * @param filePath The file path of the asset to load
         * @return The requested asset or nullptr if the loading failed
         */
        Ref<Asset> LoadAsset(AssetType assetType, const std::filesystem::path& filePath);

        /**
         * @brief
         */
        void SetAssetDirectoryPath(const std::filesystem::path& filePath) { m_AssetDirectoryPath = filePath; }

        /**
         * @brief
         */
        void SetEngineAssetDirectoryPath(const std::filesystem::path& filePath) { m_EngineAssetsDirectoryPath = filePath; }

        /**
         * @brief
         */
        AssetID GetAssetIDFromFilePath(const std::filesystem::path& filePath);

        /**
         * @brief
         */
        std::filesystem::path GetFilePathFromAssetID(AssetID assetID);

        /**
         * @brief
         */
        [[nodiscard]] const AssetRegistryStats& GetAssetRegistryStats() const { return m_RegistryStats; }

        /**
         * @brief
         */
        const std::filesystem::path& GetAssetDirectoryPath() { return m_AssetDirectoryPath; }

        /**
         * @brief
         */
        void GetRelativePath(std::filesystem::path& outfilePath); // Used to break down absolute path into relative path that is used to get assets

        /**
         * @brief
         */
        void InitAssetLoaderDefaults();

    private:

        /**
         * @brief
         */
        AssetID AssignNextAvailableAssetID();

        /**
         * @brief
         */
        void GetAbsolutePath(std::filesystem::path& outPath);

        /**
         * @brief
         */
        bool IsInDirectory(const std::filesystem::path& directory, const std::filesystem::path& assetFile);


        AssetID m_NextAvailableAssetID{0};

        std::filesystem::path m_AssetDirectoryPath;
        std::filesystem::path m_EngineAssetsDirectoryPath; // Engine assets directory

        std::unordered_map<std::filesystem::path, AssetID> m_FilePathToAssetID;
        std::unordered_map<AssetID, std::filesystem::path> m_AssetIDToFilePath;
        std::unordered_map<AssetID, Ref<Asset>> m_AssetIDToAsset;

        AssetRegistryStats m_RegistryStats{};
    };

}

#include "AssetRegistry_Impl.h"
