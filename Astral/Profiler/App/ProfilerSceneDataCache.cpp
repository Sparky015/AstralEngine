/**
* @file ProfilerSceneDataCache.cpp
* @author Andrew Fagan 
* @date 9/11/26
*/

#include "ProfilerSceneDataCache.h"

#include "Core/Utilities/Loggers.h"

namespace Astral {

    void ProfilerSceneDataCache::InitializeCache(const std::filesystem::path& filePath)
    {
        m_SceneMetricsImporter.ImportMemoryProfile(filePath);

        // ----------------


        // Initialize allocation data sorted by size
        const std::vector<AllocationDataSerializeable>& allocationData = m_SceneMetricsImporter.GetSceneMetrics().GetAllocationDataOverTime();
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
        for (const std::string& stacktrace : m_SceneMetricsImporter.GetSceneMetrics().GetStacktraceStorage())
        {
            m_SceneStacktracePrefixTree.AddStacktrace(stacktrace);
        }
        // ----------------
    }


    void ProfilerSceneDataCache::InvalidateCache()
    {
        m_SceneMetricsImporter.CloseMemoryProfile();
        m_SortedBySizeAllocationData = {};
        m_SceneStacktracePrefixTree.ClearPrefixTree();
    }


    const SceneMetricsStorage& ProfilerSceneDataCache::GetSceneMetricsStorage()
    {
        return m_SceneMetricsImporter.GetSceneMetrics();
    }


    uint64 ProfilerSceneDataCache::GetSceneStartingTimePoint() const
    {
        return m_SceneMetricsImporter.GetSceneStartingTimePoint();
    }


    std::vector<std::pair<AllocationDataSerializeable, int>>& ProfilerSceneDataCache::GetAllocationDataSortedBySize()
    {
        return m_SortedBySizeAllocationData;
    }


    SceneStacktracePrefixTree& ProfilerSceneDataCache::GetStacktracePrefixTree()
    {
        return m_SceneStacktracePrefixTree;
    }


    ProfilerViewConfig& ProfilerSceneDataCache::GetViewConfig()
    {
        return m_ProfilerViewConfig;
    }

}

