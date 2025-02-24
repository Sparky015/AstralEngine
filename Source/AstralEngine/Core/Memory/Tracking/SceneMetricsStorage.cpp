/**
* @file SceneMetricsStorage.cpp
* @author Andrew Fagan
* @date 2/9/2025
*/
#include "SceneMetricsStorage.h"

namespace Core {

    SceneMetricsStorage::SceneMetricsStorage() :
        m_NumberOfSnapshotsStored(0),
        m_ExpectedSnapshotCount(0)
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
        m_AllocationDataStorage.reserve(m_ExpectedSnapshotCount);

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


    void SceneMetricsStorage::AppendSnapshot(const MemoryMetrics& mmss, const size_t allocationTime, const AllocationDataSerializeable& allocationData) // MemoryMetric snapshot
    {
        m_PeakMemoryUsageOverTime.push_back(mmss.GetPeakMemoryUsage());
        m_TotalMemoryUsageOverTime.push_back(mmss.GetTotalMemoryUsage());
        m_TotalActiveAllocationsOverTime.push_back(mmss.GetTotalActiveAllocations());
        m_TotalAllocationsOverTime.push_back(mmss.GetTotalAllocations());


        for (auto [allocatorType, value] : mmss.GetMemoryUsageByAllocatorIterable())
        {
            if (!m_MemoryUsageByAllocatorOverTime.contains(allocatorType))
            {
                m_MemoryUsageByAllocatorOverTime[allocatorType].resize(m_NumberOfSnapshotsStored);
            }

            m_MemoryUsageByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetPeakMemoryUsageByAllocatorIterable())
        {
            if (!m_PeakMemoryUsageByAllocatorOverTime.contains(allocatorType))
            {
                m_PeakMemoryUsageByAllocatorOverTime[allocatorType].resize(m_NumberOfSnapshotsStored);
            }

            m_PeakMemoryUsageByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetActiveAllocationsByAllocatorIterable())
        {
            if (!m_ActiveAllocationsByAllocatorOverTime.contains(allocatorType))
            {
                m_ActiveAllocationsByAllocatorOverTime[allocatorType].resize(m_NumberOfSnapshotsStored);
            }

            m_ActiveAllocationsByAllocatorOverTime[allocatorType].push_back(value);
        }

        for (auto [allocatorType, value] : mmss.GetTotalAllocationsByAllocatorIterable())
        {
            if (!m_TotalAllocationsByAllocatorOverTime.contains(allocatorType))
            {
                m_TotalAllocationsByAllocatorOverTime[allocatorType].resize(m_NumberOfSnapshotsStored);
            }

            m_TotalAllocationsByAllocatorOverTime[allocatorType].push_back(value);
        }



        for (auto [region, value] : mmss.GetMemoryUsageByRegionIterable())
        {
            if (!m_MemoryUsageByRegionOverTime.contains(region))
            {
                m_MemoryUsageByRegionOverTime[region].resize(m_NumberOfSnapshotsStored);
            }

            m_MemoryUsageByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetPeakMemoryUsageByRegionIterable())
        {
            if (!m_PeakMemoryUsageByRegionOverTime.contains(region))
            {
                m_PeakMemoryUsageByRegionOverTime[region].resize(m_NumberOfSnapshotsStored);
            }

            m_PeakMemoryUsageByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetActiveAllocationsByRegionIterable())
        {
            if (!m_ActiveAllocationsByRegionOverTime.contains(region))
            {
                m_ActiveAllocationsByRegionOverTime[region].resize(m_NumberOfSnapshotsStored);
            }

            m_ActiveAllocationsByRegionOverTime[region].push_back(value);
        }

        for (auto [region, value] : mmss.GetTotalAllocationsByRegionIterable())
        {
            if (!m_TotalAllocationsByRegionOverTime.contains(region))
            {
                m_TotalAllocationsByRegionOverTime[region].resize(m_NumberOfSnapshotsStored);
            }

            m_TotalAllocationsByRegionOverTime[region].push_back(value);
        }


        for (auto [threadId, value] : mmss.GetMemoryUsageByThreadIterable())
        {
            if (!m_MemoryUsageByThreadOverTime.contains(threadId))
            {
                m_MemoryUsageByThreadOverTime[threadId].resize(m_NumberOfSnapshotsStored);
            }

            m_MemoryUsageByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetPeakMemoryUsageByThreadIterable())
        {
            if (!m_PeakMemoryUsageByThreadOverTime.contains(threadId))
            {
                m_PeakMemoryUsageByThreadOverTime[threadId].resize(m_NumberOfSnapshotsStored);
            }

            m_PeakMemoryUsageByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetActiveAllocationsByThreadIterable())
        {
            if (!m_ActiveAllocationsByThreadOverTime.contains(threadId))
            {
                m_ActiveAllocationsByThreadOverTime[threadId].resize(m_NumberOfSnapshotsStored);
            }

            m_ActiveAllocationsByThreadOverTime[threadId].push_back(value);
        }

        for (auto [threadId, value] : mmss.GetTotalAllocationsByThreadIterable())
        {
            if (!m_TotalAllocationsByThreadOverTime.contains(threadId))
            {
                m_TotalAllocationsByThreadOverTime[threadId].resize(m_NumberOfSnapshotsStored);
            }

            m_TotalAllocationsByThreadOverTime[threadId].push_back(value);
        }

        m_FrameAllocationDataStorageOverTime.push_back(mmss.GetFrameAllocationData());

        m_AllocationTimes.push_back(allocationTime);

        m_AllocationDataStorage.push_back(allocationData);

        m_NumberOfSnapshotsStored++;
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
