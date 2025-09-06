/**
* @file AssetRegistry.cpp
* @author Andrew Fagan
* @date 4/11/25
*/

#include "AssetRegistry.h"

#include "Debug/Utilities/Error.h"
#include "Loaders/CubeLUTLoader.h"
#include "Loaders/EnvironmentMapLoader.h"
#include "Loaders/TextureLoader.h"
#include "Loaders/MaterialLoader.h"
#include "Loaders/MeshLoader.h"
#include "Loaders/SceneLoader.h"
#include "Loaders/ShaderLoader.h"

namespace Astral {

    AssetRegistry::~AssetRegistry()
    {
    }


    void AssetRegistry::Init()
    {
        SetEngineAssetDirectoryPath(ASTRAL_RENDERER_ASSETS_DIR);
    }


    void AssetRegistry::Shutdown()
    {
        SceneLoader::DestroyDefaultMaterial();
    }


    void AssetRegistry::LoadScene(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("AssetRegistry::LoadScene")

        if (filePath.is_relative())
        {
            std::filesystem::path fullPath = filePath;
            GetAbsolutePath(fullPath);
            ASSERT(std::filesystem::exists(fullPath), "Given scene file path does not exist! (" << fullPath << ")")
            SceneLoader::LoadSceneAssets(fullPath);
        }
        else
        {
            ASSERT(std::filesystem::exists(filePath), "Given scene file path does not exist! (" << filePath << ")")
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
            case AssetType::EnvironmentMap: return EnvironmentMapLoader::LoadAsset(filePath);
            case AssetType::CubeLUT: return CubeLUTLoader::LoadAsset(filePath);
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


    void AssetRegistry::GetAbsolutePath(std::filesystem::path& outPath)
    {
        if (outPath.is_absolute()) { return; }

        if (IsInDirectory(m_AssetDirectoryPath, outPath))
        {
            // Path is contained in assets directory
            outPath = m_AssetDirectoryPath / outPath;
            return;
        }

        if (IsInDirectory(m_EngineAssetsDirectoryPath, outPath))
        {
            // Path is contained in engine assets directory
            outPath = m_EngineAssetsDirectoryPath / outPath;
            return;
        }

        WARN("Unable to identify parent directory of relative path! Using executable directory!")
    }


    bool AssetRegistry::IsInDirectory(const std::filesystem::path& directory, const std::filesystem::path& assetFile)
    {
        if (directory.empty() || assetFile.empty()) { return false; }
        std::filesystem::path canonicalDirectory = std::filesystem::weakly_canonical(directory);
        std::filesystem::path canonicalAssetPath;

        if (assetFile.is_relative())
        {
            canonicalAssetPath = std::filesystem::weakly_canonical(directory / assetFile);
            return std::filesystem::exists(canonicalAssetPath);
        }
        else
        {
           canonicalAssetPath = std::filesystem::weakly_canonical(assetFile);
        }


        auto dirIt = canonicalDirectory.begin();
        auto filePathIt = canonicalAssetPath.begin();

        for (;dirIt != canonicalDirectory.end() && filePathIt != canonicalAssetPath.end(); ++dirIt, ++filePathIt)
        {
            if (*dirIt != *filePathIt)
            {
                // Asset file is not within asset directory
                return false;
            }
        }

        return canonicalDirectory != canonicalAssetPath && dirIt == canonicalDirectory.end();
    }


    void AssetRegistry::GetRelativePath(std::filesystem::path& outfilePath)
    {
        if (outfilePath.string().find("Temp://") != std::string::npos) { return; } // If path has "Temp://", it means it is a runtime created resource

        if (IsInDirectory(m_AssetDirectoryPath, outfilePath))
        {
            // Path is contained in assets directory
            outfilePath = std::filesystem::proximate(outfilePath, m_AssetDirectoryPath);
            return;
        }

        if (IsInDirectory(m_EngineAssetsDirectoryPath, outfilePath))
        {
            // Path is contained in engine assets directory
            outfilePath = std::filesystem::proximate(outfilePath, m_EngineAssetsDirectoryPath);
        }
    }


    void AssetRegistry::InitAssetLoaderDefaults()
    {
        SceneLoader::InitDefaultMaterial();
    }

}
