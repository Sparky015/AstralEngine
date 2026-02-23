/**
* @file AsyncLoadRegistry.cpp
* @author Andrew Fagan
* @date 2/23/26
*/

#include "AsyncLoadRegistry.h"

#include "Debug/Utilities/Loggers.h"

namespace Astral {

    void AsyncLoadRegistry::RegisterAsyncLoad(AssetID placeholderAssetID, const std::filesystem::path& filePath, std::future<Ref<Asset>>&& assetFuture)
    {
        if (m_AssetIDToAssetFuture.contains(placeholderAssetID) || m_RegisteredFilePaths.contains(filePath))
        {
            AE_WARN("AsyncLoadRegistry already contains asset with given file path or placeholder ID!")
            return;
        }
        m_AssetIDToAssetFuture[placeholderAssetID] = std::move(assetFuture);
        m_RegisteredFilePaths.insert(filePath);
    }


    bool AsyncLoadRegistry::IsReady(AssetID placeholderAssetID)
    {
        if (!m_AssetIDToAssetFuture.contains(placeholderAssetID))
        {
            AE_WARN("AsyncLoadRegistry does not hold a asset with the placeholder asset ID of " << placeholderAssetID)
            return false;
        }

        std::future_status status = m_AssetIDToAssetFuture[placeholderAssetID].wait_for(std::chrono::seconds(0));
        return status == std::future_status::ready;
    }


    bool AsyncLoadRegistry::IsRegistered(const std::filesystem::path& filePath)
    {
        return m_RegisteredFilePaths.contains(filePath);
    }
}

