/**
* @file AssetRegistry_Impl.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/Engine.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Asserts.h"

namespace Astral {

    template<typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::CreateAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("AssetRegistry::CreateAsset")

        std::unique_lock lock(m_RegistryMutex); // Lock for the read checking if the asset is cached already

        // Check if the asset is already loaded first, if it is, return the current AssetID
        if (m_FilePathToAssetID.contains(filePath)) { return GetAsset<AssetType>(m_FilePathToAssetID.at(filePath)); }

        lock.unlock(); // Unlock for asset loading

        // Load the asset from disk
        Ref<Asset> asset;
        if (filePath.is_relative())
        {
            std::filesystem::path fullPath = filePath;
            GetAbsolutePath(fullPath);
            if (!std::filesystem::exists(fullPath))
            {
                if (fullPath == "") { return nullptr; }
                AE_WARN("Trying to create asset from file path that does not exist! (\"" << fullPath.string() << "\")")
                return nullptr;
            }
            asset = LoadAsset(AssetType::GetStaticAssetType(), fullPath);
        }
        else
        {
            if (!std::filesystem::exists(filePath))
            {
                if (filePath == "") { return nullptr; }
                AE_WARN("Trying to create asset from file path that does not exist! (" << filePath.string() << ")")
                return nullptr;
            }
            asset = LoadAsset(AssetType::GetStaticAssetType(), filePath);
        }

        if (!asset)
        {
            AE_WARN("An asset failed to load! File name: " << m_AssetDirectoryPath.string() + filePath.string())
            return nullptr;
        }

        lock.lock(); // Relock for read/writes in registering the asset to the registry

        // If the file has never been loaded, assign a new AssetID
        AssetID assetID = AssignNextAvailableAssetID();

        m_RegistryStats.NumberOfLoadsMade++;
        m_RegistryStats.NumberOfLoadedAssets++;
        m_RegistryStats.LoadedAssetsByType[AssetType::GetStaticAssetType()] += 1;

        // Now add the asset to storage
        m_FilePathToAssetID[filePath] = assetID;
        m_AssetIDToFilePath[assetID] = filePath;
        m_AssetIDToAsset[assetID] = asset;

        asset->SetAssetID(assetID);

        return GetAsset<AssetType>(assetID);
    }


    template<typename AssetType> requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::CreateAssetAsync(const std::filesystem::path& filePath)
    {
        std::unique_lock lock(m_RegistryMutex); // Lock for the thread pool access and asset ID generation access

        // Submit the asset load as an async task to the thread pool
        std::future<Ref<Asset>> futureAsset = m_ThreadPool.SubmitTaskWithResult<Ref<Asset>>([filePath, this]() {

            // Load the asset from disk
            Ref<Asset> asset = nullptr;
            if (filePath.is_relative())
            {
                std::filesystem::path fullPath = filePath;
                GetAbsolutePath(fullPath);
                if (!std::filesystem::exists(fullPath))
                {
                    if (fullPath == "") { return asset; }
                    AE_WARN("Trying to create asset from file path that does not exist! (\"" << fullPath.string() << "\")")
                    return asset;
                }
                asset = LoadAsset(AssetType::GetStaticAssetType(), fullPath);
            }
            else
            {
                if (!std::filesystem::exists(filePath))
                {
                    if (filePath == "") { return asset; }
                    AE_WARN("Trying to create asset from file path that does not exist! (" << filePath.string() << ")")
                    return asset;
                }
                asset = LoadAsset(AssetType::GetStaticAssetType(), filePath);
            }

            return asset;
        }, 1.0f);

        // Generate the placeholder asset
        AssetID placeholderID = AssignNextAvailableAssetID();
        Ref<AssetType> placeholderAsset = CreateRef<AssetType>();
        if (!placeholderAsset) { return nullptr; } // Early out if allocation fails
        placeholderAsset->SetAssetID(placeholderID);

        // AsyncLoadRegistry stores a placeholder asset ID that is tied to this asset future
        m_AsyncLoadRegistry.RegisterAsyncLoad(placeholderID, filePath, std::move(futureAsset));

        return placeholderAsset;
    }


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    void AssetRegistry::RegisterAsset(Ref<AssetType> alreadyLoadedAsset, const std::filesystem::path& filePath)
    {
        if (alreadyLoadedAsset == nullptr || filePath == "") { return; }

        PROFILE_SCOPE("AssetRegistry::RegisterAsset")

        std::unique_lock lock(m_RegistryMutex); // Lock for the read/writes during registering

        // Check if the asset is already loaded first, if it is, cancel registration
        if (m_FilePathToAssetID.contains(filePath)) { return; }

        if (filePath.is_relative())
        {
            std::filesystem::path fullPath = filePath;
            GetAbsolutePath(fullPath);
            if (!std::filesystem::exists(fullPath))
            {
                if (fullPath == "") { return; }
                AE_WARN("Trying to register asset with file path that does not exist! (\"" << fullPath.string() << "\")")
                return;
            }
        }
        else
        {
            if (!std::filesystem::exists(filePath))
            {
                if (filePath == "") { return; }
                AE_WARN("Trying to register asset with file path that does not exist! (" << filePath.string() << ")")
                return;
            }
        }

        std::filesystem::path relativePath = filePath;
        GetRelativePath(relativePath);

        // If the file has never been loaded, assign a new AssetID
        AssetID assetID = AssignNextAvailableAssetID();

        m_RegistryStats.NumberOfLoadsMade++;
        m_RegistryStats.NumberOfLoadedAssets++;
        m_RegistryStats.LoadedAssetsByType[AssetType::GetStaticAssetType()] += 1;

        // Now add the asset to storage
        m_FilePathToAssetID[relativePath] = assetID;
        m_AssetIDToFilePath[assetID] = relativePath;
        m_AssetIDToAsset[assetID] = alreadyLoadedAsset;

        alreadyLoadedAsset->SetAssetID(assetID);
    }


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    void AssetRegistry::RegisterRuntimeAsset(Ref<AssetType> alreadyLoadedAsset, const std::string& uniqueIdentifier)
    {
        if (alreadyLoadedAsset == nullptr || uniqueIdentifier == "") { return; }
        PROFILE_SCOPE("AssetRegistry::RegisterRuntimeAsset")

        std::string filePath = "Temp://" + uniqueIdentifier;

        std::unique_lock lock(m_RegistryMutex); // Lock for the read/writes during registering

        // Check if the asset is already loaded first, if it is, cancel registration
        if (m_FilePathToAssetID.contains(filePath)) { return; }


        // If the file has never been loaded, assign a new AssetID
        AssetID assetID = AssignNextAvailableAssetID();

        m_RegistryStats.NumberOfLoadsMade++;
        m_RegistryStats.NumberOfLoadedAssets++;
        m_RegistryStats.LoadedAssetsByType[AssetType::GetStaticAssetType()] += 1;

        // Now add the asset to storage
        m_FilePathToAssetID[filePath] = assetID;
        m_AssetIDToFilePath[assetID] = filePath;
        m_AssetIDToAsset[assetID] = alreadyLoadedAsset;

        alreadyLoadedAsset->SetAssetID(assetID);
    }


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::GetAsset(AssetID assetID)
    {
        ASSERT(assetID != NullAssetID, "Asset ID is null!");
        if (!m_AssetIDToAsset.contains(assetID))
        {
            AE_ERROR("Asset registry does not contain an asset with given asset ID!");
            return nullptr;
        }
        return std::dynamic_pointer_cast<AssetType>(m_AssetIDToAsset.at(assetID));
    }


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::GetAsset(std::string_view filePath)
    {
        if (!m_FilePathToAssetID.contains(filePath)) { return nullptr; }
        AssetID assetID = m_FilePathToAssetID[filePath];
        return GetAsset<AssetType>(assetID);
    }

}
