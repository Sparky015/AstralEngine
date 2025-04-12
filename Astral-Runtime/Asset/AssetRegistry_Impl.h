/**
* @file AssetRegistry_Impl.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once
#include "Debug/Utilities/Asserts.h"
#include "Renderer/Primitives/Texture.h"

namespace Astral {

    template<typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    AssetID AssetRegistry::CreateAsset(const std::filesystem::path& filePath)
    {
        // Load the asset from disk
        Asset* asset = AssetType::Create();
        ASSERT(asset, "Created asset failed to allocate memory!")

        AssetErrorCode errorCode = asset->LoadData(m_AssetDirectoryPath.string() + filePath.string());

        // Handle any errors
        if (errorCode == AssetErrorCode::FAILED_TO_LOAD_RESOURCE) [[unlikely]]
        {
            WARN("An asset failed to load! File name: " << filePath.filename())
        }
        if (errorCode != AssetErrorCode::SUCCESS) [[unlikely]]
        {
            return NullAssetID;
        }

        // Check if the asset is already loaded first, if it is, return the current AssetID
        if (m_FilePathToAssetID.contains(filePath)) { return m_FilePathToAssetID.at(filePath); }

        // If the file has never been loaded, assign a new AssetID
        AssetID assetID = AssignNextAvailableAssetID();

        m_RegistryStats.NumberOfLoadsMade++;
        m_RegistryStats.NumberOfLoadedAssets++;

        // Now add the asset to storage
        m_FilePathToAssetID[filePath] = assetID;
        m_AssetIDToFilePath[assetID] = filePath;
        m_AssetIDToAsset[assetID] = asset;

        return assetID;
    }

    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    AssetType* AssetRegistry::GetAsset(AssetID assetID)
    {
        if (!m_AssetIDToAsset.contains(assetID)) { return nullptr; }
        return (AssetType*)m_AssetIDToAsset.at(assetID);
    }

}
