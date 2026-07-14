/**
* @file AssetRegistry_Impl.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/Engine.h"
#include "Profiler/Instrumentation/ScopeProfiler.h"
#include "Core/Utilities/Asserts.h"

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

        Ref<AssetType> asset = LoadAssetWithChecks<AssetType>(filePath);

        RegisterAsset(asset, filePath); // RegisterAsset function has its own locks for thread safety

        if (!asset)
        {
            AE_WARN("Asset failed to load in CreateAsset!")
        }

        return asset;
    }


    template<typename AssetType> requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::CreateAssetAsync(const std::filesystem::path& filePath)
    {
        std::unique_lock lock(m_RegistryMutex); // Lock for the thread pool access and asset ID generation access

        // Check if the asset is already loaded first, if it is, return the current AssetID
        if (m_FilePathToAssetID.contains(filePath)) { return GetAsset<AssetType>(m_FilePathToAssetID.at(filePath)); }

        lock.unlock();

        // Submit the asset load as an async task to the thread pool
        JobManager& jobManager = Engine::Get().GetJobManager();
        std::future<Ref<Asset>> futureAsset = jobManager.GetThreadPool().SubmitTaskWithResult<Ref<Asset>>([filePath, this]() {

            // Load the asset from disk
            return LoadAssetWithChecks<AssetType>(filePath);

        }, 1.0f);

        // Generate the placeholder asset (placeholder asset is used to query status of async load)
        Ref<AssetType> placeholderAsset = CreateRef<AssetType>();
        if (!placeholderAsset) { return nullptr; } // Early out if allocation fails

        // Register the placeholder asset to the file path for future loads (RegisterAsset function has its own locks for thread safety)
        RegisterAsset(placeholderAsset, filePath);

        // AsyncLoadRegistry stores a placeholder asset ID that is tied to this asset future
        lock.lock();
        m_AsyncLoadRegistry.RegisterAsyncLoad(placeholderAsset->GetAssetID(), filePath, std::move(futureAsset));

        return placeholderAsset;
    }


    template<typename AssetType> requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::FetchAsyncLoadResult(Ref<Asset> asyncLoadPlaceholder)
    {
        if (!asyncLoadPlaceholder)
        {
            AE_WARN("Given async load placer holder is null! Skipping result fetch!")
            return nullptr;
        }
        if (!m_AsyncLoadRegistry.IsReady(asyncLoadPlaceholder->GetAssetID()))
        {
            AE_WARN("Tried to fetch result of async load that is not finished! Skipping!")
            return nullptr;
        }

        Ref<Asset> createdAsset = m_AsyncLoadRegistry.GetAsyncLoadResult(asyncLoadPlaceholder->GetAssetID());

        if (!createdAsset)
        {
            AE_WARN("Async asset load failed! (Asset is nullptr)")
            return nullptr;
        }

        Ref<AssetType> createdAssetDerivedType = std::static_pointer_cast<AssetType>(createdAsset);

        if (!createdAssetDerivedType)
        {
            AE_WARN("Failed to pointer cast to derived ref asset type!")
            return createdAssetDerivedType;
        }

        Ref<AssetType> placeholderAsset = GetAsset<AssetType>(asyncLoadPlaceholder->GetAssetID());
        if (placeholderAsset != nullptr)
        {
            *placeholderAsset = std::move(*createdAssetDerivedType); // Move the real asset into the placeholder's memory
        }
        else
        {
            // Unload the placeholder from the asset registry if the async load failed to allow for load retries
            UnloadAsset(asyncLoadPlaceholder->GetAssetID());
        }

        return placeholderAsset;
    }


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    void AssetRegistry::RegisterAsset(Ref<AssetType> alreadyLoadedAsset, const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("AssetRegistry::RegisterAsset")

        if (alreadyLoadedAsset == nullptr || filePath == "")
        {
            AE_WARN("Asset Registry: Can not register empty asset and/or with an empty file path!")
            return;
        }

        std::unique_lock lock(m_RegistryMutex); // Lock for the read/writes during registering

        // Check if the asset is already loaded first, if it is, cancel registration
        if (m_FilePathToAssetID.contains(filePath)) { return; }

        // Check if the asset file path exists on disk, else skip registering it
        if (!DoesAssetFilePathExist(filePath)) { return; }

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
        if (!m_FilePathToAssetID.contains(filePath))
        {
            AE_WARN("Failed to get asset of file path: " << filePath)
            return nullptr;
        }
        AssetID assetID = m_FilePathToAssetID[filePath];
        return GetAsset<AssetType>(assetID);
    }


    template<typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::LoadAssetWithChecks(const std::filesystem::path& filePath)
    {
        std::filesystem::path fullPath = filePath;
        GetAbsolutePath(fullPath);
        if (!DoesAssetFilePathExist(fullPath)) { return nullptr; }

        Ref<Asset> asset = LoadAsset(AssetType::GetStaticAssetType(), fullPath);

        if (!asset)
        {
            AE_WARN("An asset failed to load! File name: " << m_AssetDirectoryPath.string() + filePath.string())
            return nullptr;
        }

        Ref<AssetType> assetDerivedType = std::static_pointer_cast<AssetType>(asset);

        if (!assetDerivedType)
        {
            AE_WARN("Failed to pointer cast to derived ref asset type!")
            return assetDerivedType;
        }

        return assetDerivedType;
    }

}
