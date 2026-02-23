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
         * @brief Loads a scene at the given file path and registers all assets
         * @param filePath The file path of the scene
         */
        void LoadScene(const std::filesystem::path& filePath);

        /**
         * @brief Serializes a scene to disk
         * @param filePath The file path to write the scene file to
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
         * @brief Sets the user asset directory for the asset registry
         * @param filePath The file path of the user asset directory
         */
        void SetAssetDirectoryPath(const std::filesystem::path& filePath) { m_AssetDirectoryPath = filePath; }

        /**
         * @brief Sets the engine asset directory for the asset registry
         * @param filePath The file path of the engine asset directory
         */
        void SetEngineAssetDirectoryPath(const std::filesystem::path& filePath) { m_EngineAssetsDirectoryPath = filePath; }

        /**
         * @brief Gets the asset ID of a registered asset from the associated file path
         * @param filePath The file path the asset is associated with
         * @return The asset ID of the asset associated with the given file path or the null asset ID if the file path
         *         is not associated with an asset in the registry
         */
        AssetID GetAssetIDFromFilePath(const std::filesystem::path& filePath);

        /**
         * @brief Gets the associated file path to a registered asset
         * @param assetID The ID of the asset to get the file path for
         * @return The file path of the requested asset or an empty path if the asset is not in the asset registry
         */
        std::filesystem::path GetFilePathFromAssetID(AssetID assetID);

        /**
         * @brief Gets the tracked load and unload statistics for the asset registry
         * @return All tracked asset registry statistics
         */
        [[nodiscard]] const AssetRegistryStats& GetAssetRegistryStats() const { return m_RegistryStats; }

        /**
         * @brief Gets the set asset directory
         */
        const std::filesystem::path& GetAssetDirectoryPath() { return m_AssetDirectoryPath; }

        /**
         * @brief Takes an absolute filepath and finds the relative filepath to the set asset directory and engine asset directory if any
         * @param outfilePath The absolute file path to transform into relative file path
         */
        void GetRelativePath(std::filesystem::path& outfilePath); // Used to break down absolute path into relative path that is used to get assets

        /**
         * @brief Initializes the fallback defaults of asset loading functions
         */
        void InitAssetLoaderDefaults();

    private:

        /**
         * @brief Generates the next asset ID to use when registering an asset
         * @return A generated asset ID to use when registering an asset
         */
        AssetID AssignNextAvailableAssetID();

        /**
         * @brief Gets the full absolute file path from a given relative file path that is in either the set user asset or engine asset directories
         * @param outPath A relative file path inside the set user asset directory or engine asset directory
         */
        void GetAbsolutePath(std::filesystem::path& outPath);

        /**
         * @brief Recursively checks if a given file path is inside the given directory
         * @param directory The directory to check if the given file path is in
         * @param assetFile The file path to check if it is in the given directory
         * @return True if the given directory contains the given file path, false otherwise
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
