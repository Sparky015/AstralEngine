/**
* @file AsyncLoadRegistry.h
* @author Andrew Fagan
* @date 2/23/26
*/

#pragma once


#include "Asset.h"
#include "Ref.h"

#include <unordered_map>
#include <future>

namespace Astral {

    /**
     * @brief Stores asset futures in a common place to track asynchronous asset loads and their results
     */
    class AsyncLoadRegistry
    {
    public:

        /**
         * @brief Registers an async asset load to store the future of the asset for future reference
         * @param placeholderAssetID The asset ID of the placeholder that is used to query the state of the async load
         * @param assetFuture The future of the asset to get the status and result of the async load
         */
        void RegisterAsyncLoad(AssetID placeholderAssetID, const std::filesystem::path& filePath, std::future<Ref<Asset>>&& assetFuture);

        /**
         * @brief Checks if an asset is ready to be retrieved
         * @param placeholderAssetID The asset ID of the placeholder
         * @return True is the asset is ready to be retrieved, false if the asset is loading or was never registered
         */
        bool IsReady(AssetID placeholderAssetID);

        /**
         * @brief Retreives the result of an async load
         * @param placeholderAssetID The asset ID of the placeholder associated with the async load
         * @return The result of the async load or nullptr if the async load is not finished
         */
        Ref<Asset> GetAsyncLoadResult(AssetID placeholderAssetID);

        /**
         * @brief Checks if the file path of an asset is currently registered in the async load registry
         * @param filePath The file path of asset loading
         * @return True if the file path is registered and false otherwise
         */
        bool IsRegistered(const std::filesystem::path& filePath);

        /**
         * @brief Checks if the file path of an asset is currently registered in the async load registry
         * @param placeholderAssetID The asset ID of the placeholder
         * @return True if the file path is registered and false otherwise
         */
        bool IsRegistered(AssetID placeholderAssetID);


        const std::filesystem::path& GetFilePathAssociatedWithPlaceHolder(AssetID placeholderID);


    private:

        void UnregisterAsyncLoad(AssetID placeholderAssetID);

        std::unordered_map<AssetID, std::future<Ref<Asset>>> m_AssetIDToAssetFuture;
        std::unordered_map<AssetID, std::filesystem::path> m_AssetIDToFilePaths;
        std::unordered_set<std::filesystem::path> m_RegisteredFilePaths;
    };

}
