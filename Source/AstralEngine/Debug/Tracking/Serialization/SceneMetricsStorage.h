/**
* @file SceneMetricsStorage.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "Debug/Tracking/AllocationData.h"
#include "../NoTrackingSTLAllocator.h"
#include <vector>

#include "Debug/Tracking/MemoryMetrics.h"


namespace Core {

    class SceneMetricsStorage
    {
    public:
        SceneMetricsStorage();
        ~SceneMetricsStorage();

        using DataPointStorage = std::vector<size_t>;
        using AllocatorTypeStorageMap = std::unordered_map<AllocatorType, DataPointStorage, std::hash<AllocatorType>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const AllocatorType, DataPointStorage>>>;
        using MemoryRegionStorageMap = std::unordered_map<MemoryRegion, DataPointStorage, std::hash<MemoryRegion>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const MemoryRegion, DataPointStorage>>>;
        using ThreadStorageMap = std::unordered_map<size_t, DataPointStorage, std::hash<size_t>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const size_t, DataPointStorage>>>;

        struct AllocationDataStorage
        {
            std::vector<uintptr_t> PointerStorage{};
            std::vector<size_t> SizeStorage{};
            std::vector<MemoryRegion> RegionStorage{};
            std::vector<AllocatorType> AllocatorTypeStorage{};
            std::vector<size_t> ThreadIDStorage{};
        };

        void InitializeStorage(size_t numberOfSnapshots);
        void AppendSnapshot(const MemoryMetrics& memoryMetricsSnapshot, size_t allocationTime, const AllocationDataSerializeable& allocationData, const std::string& stacktrace);


        [[nodiscard]] const DataPointStorage& GetGlobalMemoryUsageOverTime() const { return m_TotalMemoryUsageOverTime; }
        [[nodiscard]] const DataPointStorage& GetGlobalPeakMemoryUsageOverTime() const { return m_PeakMemoryUsageOverTime; }
        [[nodiscard]] const DataPointStorage& GetGlobalTotalAllocationsOverTime() const { return m_TotalAllocationsOverTime; }
        [[nodiscard]] const DataPointStorage& GetGlobalActiveAllocationsOverTime() const { return m_TotalActiveAllocationsOverTime; }

        [[nodiscard]] const DataPointStorage& GetAllocatorMemoryUsageOverTime(AllocatorType type) const { return m_MemoryUsageByAllocatorOverTime.at(type); }
        [[nodiscard]] const DataPointStorage& GetAllocatorPeakMemoryUsageOverTime(AllocatorType type) const { return m_PeakMemoryUsageByAllocatorOverTime.at(type); }
        [[nodiscard]] const DataPointStorage& GetAllocatorActiveAllocationsOverTime(AllocatorType type) const { return m_ActiveAllocationsByAllocatorOverTime.at(type); }
        [[nodiscard]] const DataPointStorage& GetAllocatorTotalAllocationsOverTime(AllocatorType type) const { return m_TotalAllocationsByAllocatorOverTime.at(type); }

        [[nodiscard]] const AllocatorTypeStorageMap& GetAllocatorMemoryUsageOverTimeIterable() const { return m_MemoryUsageByAllocatorOverTime; }
        [[nodiscard]] const AllocatorTypeStorageMap& GetAllocatorPeakMemoryUsageOverTimeIterable() const { return m_PeakMemoryUsageByAllocatorOverTime; }
        [[nodiscard]] const AllocatorTypeStorageMap& GetAllocatorActiveAllocationsOverTimeIterable() const { return m_ActiveAllocationsByAllocatorOverTime; }
        [[nodiscard]] const AllocatorTypeStorageMap& GetAllocatorTotalAllocationsOverTimeIterable() const { return m_TotalAllocationsByAllocatorOverTime; }

        [[nodiscard]] const DataPointStorage& GetRegionMemoryUsageOverTime(MemoryRegion region) const { return m_MemoryUsageByRegionOverTime.at(region); }
        [[nodiscard]] const DataPointStorage& GetRegionPeakMemoryUsageOverTime(MemoryRegion region) const { return m_PeakMemoryUsageByRegionOverTime.at(region); }
        [[nodiscard]] const DataPointStorage& GetRegionActiveAllocationsOverTime(MemoryRegion region) const { return m_ActiveAllocationsByRegionOverTime.at(region); }
        [[nodiscard]] const DataPointStorage& GetRegionTotalAllocationsOverTime(MemoryRegion region) const { return m_TotalAllocationsByRegionOverTime.at(region); }

        [[nodiscard]] const MemoryRegionStorageMap& GetRegionMemoryUsageOverTimeIterable() const { return m_MemoryUsageByRegionOverTime; }
        [[nodiscard]] const MemoryRegionStorageMap& GetRegionPeakMemoryUsageOverTimeIterable() const { return m_PeakMemoryUsageByRegionOverTime; }
        [[nodiscard]] const MemoryRegionStorageMap& GetRegionActiveAllocationsOverTimeIterable() const { return m_ActiveAllocationsByRegionOverTime; }
        [[nodiscard]] const MemoryRegionStorageMap& GetRegionTotalAllocationsOverTimeIterable() const { return m_TotalAllocationsByRegionOverTime; }

        [[nodiscard]] const DataPointStorage& GetThreadMemoryUsageOverTime(size_t threadId) const { return m_MemoryUsageByThreadOverTime.at(threadId); }
        [[nodiscard]] const DataPointStorage& GetThreadPeakMemoryUsageOverTime(size_t threadId) const { return m_PeakMemoryUsageByThreadOverTime.at(threadId); }
        [[nodiscard]] const DataPointStorage& GetThreadActiveAllocationsOverTime(size_t threadId) const { return m_ActiveAllocationsByThreadOverTime.at(threadId); }
        [[nodiscard]] const DataPointStorage& GetThreadTotalAllocationsOverTime(size_t threadId) const { return m_TotalAllocationsByThreadOverTime.at(threadId); }

        [[nodiscard]] const ThreadStorageMap& GetThreadMemoryUsageOverTimeIterable() const { return m_MemoryUsageByThreadOverTime; }
        [[nodiscard]] const ThreadStorageMap& GetThreadPeakMemoryUsageOverTimeIterable() const { return m_PeakMemoryUsageByThreadOverTime; }
        [[nodiscard]] const ThreadStorageMap& GetThreadActiveAllocationsOverTimeIterable() const { return m_ActiveAllocationsByThreadOverTime; }
        [[nodiscard]] const ThreadStorageMap& GetThreadTotalAllocationsOverTimeIterable() const { return m_TotalAllocationsByThreadOverTime; }

        [[nodiscard]] const DataPointStorage& GetAllocationTimes() const { return m_AllocationTimes; }

        [[nodiscard]] const std::vector<AllocationDataSerializeable>& GetAllocationDataOverTime() const { return m_AllocationDataStorage; }

        [[nodiscard]] const std::vector<std::string>& GetStacktraceStorage() const { return m_Stacktraces; }


        [[nodiscard]] size_t GetSnapshotCount() const { return m_NumberOfSnapshotsStored; }
        [[nodiscard]] size_t GetExpectedSnapshotCount() const { return m_ExpectedSnapshotCount; }

        void ResetStorage();

    private:

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

        std::vector<AllocationDataSerializeable> m_AllocationDataStorage;

        std::vector<std::string> m_Stacktraces;

        DataPointStorage m_AllocationTimes;

        // TODO: rest of the data in vectors
    };

}
