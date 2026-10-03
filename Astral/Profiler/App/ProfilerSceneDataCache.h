/**
* @file ProfilerSceneDataCache.h
* @author Andrew Fagan 
* @date 9/11/26
*/

#pragma once

#include "Profiler/MemoryTracking/AllocationData.h"
#include "Profiler/MemoryTracking/Serialization/SceneMetricsStorage.h"

#include <vector>

#include "SceneStacktracePrefixTree.h"

namespace Astral {

    class ProfilerSceneDataCache
    {
    public:

        /**
         * @brief Initializes the scene data cache with the memory profiles scene metrics storage
         * @param sceneMetricsStorage A pointer to the scene metrics storage. Pointer is required to be valid while memory profile is loaded
         */
        void InitializeCache(const SceneMetricsStorage* sceneMetricsStorage);

        /**
         * @brief Invalidates the scene data cache
         */
        void InvalidateCache();

        /**
         * @brief Gets the scene metrics of the loaded profile
         * @return The scene metrics of the loaded profile
         */
        const SceneMetricsStorage& GetSceneMetricsStorage();

        /**
         * @brief Gets the allocation data of the scene sorted by allocation size
         * @return The allocation data of the scene sorted by allocation size
         * @note Container to cache sorted allocation data with .second containing a index mapping to the original time-based allocation data order
         */
        std::vector<std::pair<AllocationDataSerializeable, int>>& GetAllocationDataSortedBySize();

        /**
         * @brief Gets the stacktrace prefix tree
         * @return The stacktrace prefix tree
         */
        SceneStacktracePrefixTree& GetStacktracePrefixTree();

    private:

        const SceneMetricsStorage* m_SceneMetricsStorage = nullptr;
        std::vector<std::pair<AllocationDataSerializeable, int>> m_SortedBySizeAllocationData = {};
        SceneStacktracePrefixTree m_SceneStacktracePrefixTree = {};
    };

}
