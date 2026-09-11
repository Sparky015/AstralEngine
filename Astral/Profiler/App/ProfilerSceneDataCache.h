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
        void InitializeCache(SceneMetricsStorage* sceneMetricsStorage);

        /**
         * @brief Invalidates the scene data cache
         */
        void InvalidateCache();

        /**
         * @brief Gets the scene metrics of the loaded profile
         * @return The scene metrics of the loaded profile
         */
        SceneMetricsStorage& GetSceneMetricsStorage();

        /**
         * @brief Gets the allocation data of the scene sorted by allocation size
         * @return The allocation data of the scene sorted by allocation size
         */
        std::vector<std::pair<AllocationDataSerializeable, int>>& GetAllocationDataSortedBySize();

    private:

        SceneMetricsStorage* m_SceneMetricsStorage = nullptr;
        std::vector<std::pair<AllocationDataSerializeable, int>> m_SortedBySizeAllocationData = {};
        SceneStacktracePrefixTree m_SceneStacktracePrefixTree = {};
    };

}
