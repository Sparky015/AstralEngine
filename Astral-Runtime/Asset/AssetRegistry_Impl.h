/**
* @file AssetRegistry_Impl.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Asserts.h"

namespace Astral {

    template<typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::CreateAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("AssetRegistry::CreateAsset")

        // Check if the asset is already loaded first, if it is, return the current AssetID
        if (m_FilePathToAssetID.contains(filePath)) { return GetAsset<AssetType>(m_FilePathToAssetID.at(filePath)); }

        // Load the asset from disk
        Ref<Asset> asset;
        if (filePath.is_relative())
        {
            std::filesystem::path fullPath = filePath;
            GetAbsolutePath(fullPath);
            if (!std::filesystem::exists(fullPath))
            {
                if (fullPath == "") { return nullptr; }
                WARN("Trying to create asset from file path that does not exist! (\"" << fullPath.string() << "\")")
                return nullptr;
            }
            asset = LoadAsset(AssetType::GetStaticAssetType(), fullPath);
        }
        else
        {
            if (!std::filesystem::exists(filePath))
            {
                if (filePath == "") { return nullptr; }
                WARN("Trying to create asset from file path that does not exist! (" << filePath.string() << ")")
                return nullptr;
            }
            asset = LoadAsset(AssetType::GetStaticAssetType(), filePath);
        }

        if (!asset)
        {
            WARN("An asset failed to load! File name: " << m_AssetDirectoryPath.string() + filePath.string())
            return nullptr;
        }



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


    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    Ref<AssetType> AssetRegistry::GetAsset(AssetID assetID)
    {
        ASSERT(assetID != NullAssetID, "Asset ID is null!");
        if (!m_AssetIDToAsset.contains(assetID))
        {
            ASTRAL_ERROR("Asset registry does not contain an asset with given asset ID!");
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
