/**
* @file SceneMetricsStorage.cpp
* @author Andrew Fagan
* @date 2/9/2025
*/
#include "SceneMetricsStorage.h"

namespace Core {

    SceneMetricsStorage::SceneMetricsStorage() :
        m_NumberOfSnapshotsStored(0)
    {

    }


    SceneMetricsStorage::~SceneMetricsStorage()
    {

    }


    void SceneMetricsStorage::InitializeStorage(size_t numberOfSnapshots)
    {
        m_ExpectedSnapshotCount = numberOfSnapshots;

        m_PeakMemoryUsageOverTime.reserve(m_ExpectedSnapshotCount);
        m_TotalMemoryUsageOverTime.reserve(m_ExpectedSnapshotCount);
        m_TotalAllocationsOverTime.reserve(m_ExpectedSnapshotCount);
        m_TotalActiveAllocationsOverTime.reserve(m_ExpectedSnapshotCount);

        m_FrameAllocationDataStorageOverTime.reserve(m_ExpectedSnapshotCount);
        m_AllocationTimes.reserve(m_ExpectedSnapshotCount);

        // TODO Change this to reserving space for the vector buffer and not map buffer
        // m_MemoryUsageByAllocator.reserve(m_NumberOfSnapshotsStored);
        // m_PeakMemoryUsageByAllocator.reserve(m_NumberOfSnapshotsStored);
        // m_ActiveAllocationsByAllocator.reserve(m_NumberOfSnapshotsStored);
        // m_TotalAllocationsByAllocator.reserve(m_NumberOfSnapshotsStored);
        //
        // m_MemoryUsageByRegion.reserve(m_NumberOfSnapshotsStored);
        // m_PeakMemoryUsageByRegion.reserve(m_NumberOfSnapshotsStored);
        // m_ActiveAllocationsByRegion.reserve(m_NumberOfSnapshotsStored);
        // m_TotalAllocationsByRegion.reserve(m_NumberOfSnapshotsStored);
        //
        // m_MemoryUsageByThread.reserve(m_NumberOfSnapshotsStored);
        // m_PeakMemoryUsageByThread.reserve(m_NumberOfSnapshotsStored);
        // m_ActiveAllocationsByThread.reserve(m_NumberOfSnapshotsStored);
        // m_TotalAllocationsByThread.reserve(m_NumberOfSnapshotsStored);
    }


    void SceneMetricsStorage::AppendSnapshot(const MemoryMetrics& mmss, const size_t allocationTime) // MemoryMetric snapshot
    {
        m_NumberOfSnapshotsStored++;

        m_PeakMemoryUsageOverTime.push_back(mmss.GetPeakMemoryUsage());
        m_TotalMemoryUsageOverTime.push_back(mmss.GetTotalMemoryUsage());
        m_TotalActiveAllocationsOverTime.push_back(mmss.GetTotalActiveAllocations());
        m_TotalAllocationsOverTime.push_back(mmss.GetTotalAllocations());


        for (auto [allocatorType, value] : mmss.GetMemoryUsageByAllocatorIterable())
        {
            m_MemoryUsageByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetPeakMemoryUsageByAllocatorIterable())
        {
            m_PeakMemoryUsageByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetActiveAllocationsByAllocatorIterable())
        {
            m_ActiveAllocationsByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetTotalAllocationsByAllocatorIterable())
        {
            m_TotalAllocationsByAllocatorOverTime[allocatorType].push_back(value);
        }



        for (auto [region, value] : mmss.GetMemoryUsageByRegionIterable())
        {
            m_MemoryUsageByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetPeakMemoryUsageByRegionIterable())
        {
            m_PeakMemoryUsageByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetActiveAllocationsByRegionIterable())
        {
            m_ActiveAllocationsByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetTotalAllocationsByRegionIterable())
        {
            m_TotalAllocationsByRegionOverTime[region].push_back(value);
        }



        for (auto [threadId, value] : mmss.GetMemoryUsageByThreadIterable())
        {
            m_MemoryUsageByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetPeakMemoryUsageByThreadIterable())
        {
            m_PeakMemoryUsageByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetActiveAllocationsByThreadIterable())
        {
            m_ActiveAllocationsByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetTotalAllocationsByThreadIterable())
        {
            m_TotalAllocationsByThreadOverTime[threadId].push_back(value);
        }

        m_FrameAllocationDataStorageOverTime.push_back(mmss.GetFrameAllocationData());

        m_AllocationTimes.push_back(allocationTime);
    }


    void SceneMetricsStorage::ResetStorage()
    {
        m_NumberOfSnapshotsStored = 0;
        m_ExpectedSnapshotCount = 0;

        m_PeakMemoryUsageOverTime.clear();
        m_TotalMemoryUsageOverTime.clear();
        m_TotalActiveAllocationsOverTime.clear();
        m_TotalAllocationsOverTime.clear();

        m_MemoryUsageByAllocatorOverTime.clear();
        m_PeakMemoryUsageByAllocatorOverTime.clear();
        m_ActiveAllocationsByAllocatorOverTime.clear();
        m_TotalAllocationsByAllocatorOverTime.clear();

        m_MemoryUsageByRegionOverTime.clear();
        m_PeakMemoryUsageByRegionOverTime.clear();
        m_ActiveAllocationsByRegionOverTime.clear();
        m_TotalAllocationsByRegionOverTime.clear();

        m_MemoryUsageByThreadOverTime.clear();
        m_PeakMemoryUsageByThreadOverTime.clear();
        m_ActiveAllocationsByThreadOverTime.clear();
        m_TotalAllocationsByThreadOverTime.clear();

        m_FrameAllocationDataStorageOverTime.back();

        m_AllocationTimes.clear();
    }
}
