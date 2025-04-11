/**
* @file AssetRegistry.cpp
* @author Andrew Fagan
* @date 4/11/25
*/

#include "AssetRegistry.h"

namespace Astral {

    void AssetRegistry::UnloadAsset(AssetID assetID)
    {
        if (!m_AssetIDToFilePath.contains(assetID)) { return; }
        const std::filesystem::path& assetFilePath = m_AssetIDToFilePath.at(assetID);

        UnloadAsset(assetFilePath);

        // Note that the Asset stored is a shared pointer / ref, so it will be deallocated when the last reference of it
        // goes out of scope.
    }


    void AssetRegistry::UnloadAsset(const std::filesystem::path& assetFilePath)
    {
        // Check if the asset is loaded first. Exit early if not loaded.
        AssetID assetID = GetAssetIDFromFilePath(assetFilePath);
        if (assetID == NullAssetID) { return; }

        m_RegistryStats.NumberOfUnloadsMade++;
        m_RegistryStats.NumberOfLoadedAssets--;

        m_FilePathToAssetID.erase(assetFilePath);
        m_AssetIDToFilePath.erase(assetID);
        m_AssetIDToAsset.erase(assetID);

        // Note that the Asset stored is a shared pointer / ref, so it will be deallocated when the last reference of it
        // goes out of scope.
    }


    AssetID AssetRegistry::GetAssetIDFromFilePath(const std::filesystem::path& filePath)
    {
        if (!m_FilePathToAssetID.contains(filePath)) { return NullAssetID; }
        return m_FilePathToAssetID.at(filePath);
    }


    AssetID AssetRegistry::AssignNextAvailableAssetID()
    {
        AssetID nextAssetID = m_NextAvailableAssetID;
        m_NextAvailableAssetID++;
        return nextAssetID;
    }

}
