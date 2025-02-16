/**
* @file SceneMetricsStorage.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "AllocationData.h"
#include "Core/Memory/Allocators/NoTrackingAllocator.h"
#include <vector>

#include "MemoryMetrics.h"


namespace Core {

    class SceneMetricsStorage
    {
    public:
        SceneMetricsStorage();
        ~SceneMetricsStorage();

        void InitializeStorage(size_t numberOfSnapshots);

        void AppendSnapshot(const MemoryMetrics& memoryMetricsSnapshot);

        [[nodiscard]] size_t GetSnapshotCount() const { return m_NumberOfSnapshotsStored; }
        [[nodiscard]] size_t GetExpectedSnapshotCount() const { return m_ExpectedSnapshotCount; }

    private:
        using DataPointStorage = std::vector<size_t>;

        using AllocatorTypeStorageMap = std::unordered_map<AllocatorType, DataPointStorage, std::hash<AllocatorType>, std::equal_to<>, NoTrackingAllocator<std::pair<const AllocatorType, DataPointStorage>>>;
        using MemoryRegionStorageMap = std::unordered_map<MemoryRegion, DataPointStorage, std::hash<MemoryRegion>, std::equal_to<>, NoTrackingAllocator<std::pair<const MemoryRegion, DataPointStorage>>>;
        using ThreadStorageMap = std::unordered_map<size_t, DataPointStorage, std::hash<size_t>, std::equal_to<>, NoTrackingAllocator<std::pair<const size_t, DataPointStorage>>>;

        size_t m_NumberOfSnapshotsStored;
        size_t m_ExpectedSnapshotCount;

        DataPointStorage m_PeakMemoryUsageOverTime;
        DataPointStorage m_TotalMemoryUsageOverTime;
        DataPointStorage m_TotalActiveAllocationsOverTime;
        DataPointStorage m_TotalAllocationsOverTime;

        AllocatorTypeStorageMap m_MemoryUsageByAllocatorOverTime;
        AllocatorTypeStorageMap m_PeakMemoryUsageByAllocatorOverTime;
        AllocatorTypeStorageMap m_ActiveAllocationsByAllocatorOverTime;
        AllocatorTypeStorageMap m_TotalAllocationsByAllocatorOverTime;

        MemoryRegionStorageMap m_MemoryUsageByRegionOverTime;
        MemoryRegionStorageMap m_PeakMemoryUsageByRegionOverTime;
        MemoryRegionStorageMap m_ActiveAllocationsByRegionOverTime;
        MemoryRegionStorageMap m_TotalAllocationsByRegionOverTime;

        ThreadStorageMap m_MemoryUsageByThreadOverTime;
        ThreadStorageMap m_PeakMemoryUsageByThreadOverTime;
        ThreadStorageMap m_ActiveAllocationsByThreadOverTime;
        ThreadStorageMap m_TotalAllocationsByThreadOverTime;

        std::vector<FrameAllocationData> m_FrameAllocationDataStorageOverTime;

        // TODO: rest of the data in vectors
    };

}
