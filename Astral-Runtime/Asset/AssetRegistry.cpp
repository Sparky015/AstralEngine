/**
* @file AssetRegistry.cpp
* @author Andrew Fagan
* @date 4/11/25
*/

#include "AssetRegistry.h"

#include "Debug/Utilities/Error.h"
#include "Loaders/TextureLoader.h"
#include "Loaders/MaterialLoader.h"
#include "Loaders/MeshLoader.h"
#include "Loaders/SceneLoader.h"
#include "Loaders/ShaderLoader.h"

namespace Astral {

    AssetRegistry::~AssetRegistry()
    {

    }


    void AssetRegistry::LoadScene(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("AssetRegistry::LoadScene")

        if (filePath.is_relative())
        {
            std::string fullFilePath = m_AssetDirectoryPath.string() + filePath.string();
            ASSERT(std::filesystem::exists(fullFilePath), "Given scene file path does not exist! (" << fullFilePath << ")")
            SceneLoader::LoadSceneAssets(fullFilePath);
        }
        else
        {
            ASSERT(std::filesystem::exists(filePath), "Given scene file path does not exist! (" << filePath.string() << ")")
            SceneLoader::LoadSceneAssets(filePath);
        }
    }


    void AssetRegistry::SerializeScene(Scene& scene, const std::filesystem::path& filePath)
    {
        SceneLoader::SerializeScene(scene, filePath);
    }


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

        AssetType type = m_AssetIDToAsset[assetID]->GetAssetType();
        m_RegistryStats.LoadedAssetsByType[type] -= 1;

        m_FilePathToAssetID.erase(assetFilePath);
        m_AssetIDToFilePath.erase(assetID);
        m_AssetIDToAsset.erase(assetID);

        // Note that the Asset stored is a shared pointer / ref, so it will be deallocated when the last reference of it
        // goes out of scope.
    }

    Ref<Asset> AssetRegistry::LoadAsset(AssetType assetType, const std::filesystem::path& filePath)
    {
        switch (assetType)
        {
            case AssetType::Texture: return TextureLoader::LoadAsset(filePath);
            case AssetType::Material: return MaterialLoader::LoadAsset(filePath);
            case AssetType::Shader: return ShaderLoader::LoadAsset(filePath);
            case AssetType::Mesh: return MeshLoader::LoadAsset(filePath);
            default: ASTRAL_ERROR("Invalid asset type value given!");
        }
    }


    AssetID AssetRegistry::GetAssetIDFromFilePath(const std::filesystem::path& filePath)
    {
        if (!m_FilePathToAssetID.contains(filePath)) { return NullAssetID; }
        return m_FilePathToAssetID.at(filePath);
    }


    std::filesystem::path AssetRegistry::GetFilePathFromAssetID(AssetID assetID)
    {
        if (!m_AssetIDToFilePath.contains(assetID)) { return ""; }
        return m_AssetIDToFilePath[assetID];
    }


    AssetID AssetRegistry::AssignNextAvailableAssetID()
    {
        AssetID nextAssetID = m_NextAvailableAssetID;
        m_NextAvailableAssetID++;
        return nextAssetID;
    }

}
