/**
* @file ProfilerSceneDataCache.cpp
* @author Andrew Fagan 
* @date 9/11/26
*/

#include "ProfilerSceneDataCache.h"

#include "Core/Utilities/Loggers.h"

namespace Astral {

    void ProfilerSceneDataCache::InitializeCache(const SceneMetricsStorage* sceneMetricsStorage)
    {
        if (!sceneMetricsStorage)
        {
            AE_WARN("Unable to initialize profiler scene cache with nullptr scene metrics storage!");
            return;
        }


        // Initialize reference scene metrics storage
        m_SceneMetricsStorage = sceneMetricsStorage;
        // ----------------


        // Initialize allocation data sorted by size
        const std::vector<AllocationDataSerializeable>& allocationData = sceneMetricsStorage->GetAllocationDataOverTime();
        m_SortedBySizeAllocationData.resize(allocationData.size());

        for (int i = 0; i < allocationData.size(); i++)
        {
            m_SortedBySizeAllocationData[i].first = allocationData[i];
            m_SortedBySizeAllocationData[i].second = i;
        }
        std::ranges::stable_sort(m_SortedBySizeAllocationData, [](const std::pair<AllocationDataSerializeable, int>& p1, const std::pair<AllocationDataSerializeable, int>& p2) {
            return p1.first.size < p2.first.size;
        });
        // ----------------


        // Initialize stacktrace prefix tree
        for (const std::string& stacktrace : sceneMetricsStorage->GetStacktraceStorage())
        {
            m_SceneStacktracePrefixTree.AddStacktrace(stacktrace);
        }
        // ----------------
    }


    void ProfilerSceneDataCache::InvalidateCache()
    {
        m_SceneMetricsStorage = nullptr;
        m_SortedBySizeAllocationData = {};
        m_SceneStacktracePrefixTree.ClearPrefixTree();
    }


    const SceneMetricsStorage& ProfilerSceneDataCache::GetSceneMetricsStorage()
    {
        return *m_SceneMetricsStorage;
    }


    std::vector<std::pair<AllocationDataSerializeable, int>>& ProfilerSceneDataCache::GetAllocationDataSortedBySize()
    {
        return m_SortedBySizeAllocationData;
    }


    SceneStacktracePrefixTree& ProfilerSceneDataCache::GetStacktracePrefixTree()
    {
        return m_SceneStacktracePrefixTree;
    }

}

