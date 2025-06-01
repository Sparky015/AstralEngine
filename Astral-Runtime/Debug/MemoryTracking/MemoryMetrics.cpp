//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetrics.h"

namespace Astral {

    MemoryMetrics::MemoryMetrics() :
        m_PeakMemoryUsage(0),
        m_TotalMemoryUsage(0),
        m_TotalActiveAllocations(0),
        m_TotalAllocations(0),
        m_NewFrameEventListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }}
    {
        // Initialize unordered_maps keys with all enum values
    }


    void MemoryMetrics::Init()
    {
        m_NewFrameEventListener.StartListening();
    }


    void MemoryMetrics::Shutdown()
    {
        m_NewFrameEventListener.StopListening();
    }


    void MemoryMetrics::TrackAllocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage += allocationInfo.size;
        m_PeakMemoryUsage = std::max(m_PeakMemoryUsage, m_TotalMemoryUsage);
        m_TotalActiveAllocations++;
        m_TotalAllocations++;

        const size_t threadIDHash = GetThreadIDHash(allocationInfo.threadID);

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationInfo.size;

        m_MemoryUsageByAllocator[allocationInfo.allocatorType] += allocationInfo.size;
        m_MemoryUsageByRegion[allocationInfo.region] += allocationInfo.size;
        m_MemoryUsageByThread[threadIDHash] += allocationInfo.size;

        m_ActiveAllocationsByAllocator[allocationInfo.allocatorType]++;
        m_ActiveAllocationsByRegion[allocationInfo.region]++;
        m_ActiveAllocationsByThread[threadIDHash]++;

        m_TotalAllocationsByAllocator[allocationInfo.allocatorType]++;
        m_TotalAllocationsByRegion[allocationInfo.region]++;
        m_TotalAllocationsByThread[threadIDHash]++;

        m_PeakMemoryUsageByAllocator[allocationInfo.allocatorType] = std::max(
            m_MemoryUsageByAllocator[allocationInfo.allocatorType],
            m_PeakMemoryUsageByAllocator[allocationInfo.allocatorType]);

        m_PeakMemoryUsageByRegion[allocationInfo.region] = std::max(
            m_MemoryUsageByRegion[allocationInfo.region],
            m_PeakMemoryUsageByRegion[allocationInfo.region]);


        m_PeakMemoryUsageByThread[threadIDHash] = std::max(
            m_MemoryUsageByThread[threadIDHash],
            m_PeakMemoryUsageByThread[threadIDHash]);
    }


    void MemoryMetrics::TrackDeallocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage -= allocationInfo.size;
        m_TotalActiveAllocations--;

        const size_t threadIDHash = GetThreadIDHash(allocationInfo.threadID);

        m_MemoryUsageByAllocator[allocationInfo.allocatorType] -= allocationInfo.size;
        m_MemoryUsageByRegion[allocationInfo.region] -= allocationInfo.size;
        m_MemoryUsageByThread[threadIDHash] -= allocationInfo.size;

        m_ActiveAllocationsByAllocator[allocationInfo.allocatorType]--;
        m_ActiveAllocationsByRegion[allocationInfo.region]--;
        m_ActiveAllocationsByThread[threadIDHash]--;
    }


    size_t MemoryMetrics::GetAllocatorTypeUsage(AllocatorType allocatorType) const
    {
        if (!m_MemoryUsageByAllocator.contains(allocatorType)) { return 0; }
        return m_MemoryUsageByAllocator.at(allocatorType);
    }


    size_t MemoryMetrics::GetAllocatorTypePeakUsage(AllocatorType allocatorType) const
    {
        if (!m_PeakMemoryUsageByAllocator.contains(allocatorType)) { return 0; }
        return m_PeakMemoryUsageByAllocator.at(allocatorType);
    }


    size_t MemoryMetrics::GetMemoryRegionUsage(MemoryRegion memoryRegion) const
    {
        if (!m_MemoryUsageByRegion.contains(memoryRegion)) { return 0; }
        return m_MemoryUsageByRegion.at(memoryRegion);
    }


    size_t MemoryMetrics::GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const
    {
        if (!m_PeakMemoryUsageByRegion.contains(memoryRegion)) { return 0; }
        return m_PeakMemoryUsageByRegion.at(memoryRegion);
    }


    size_t MemoryMetrics::GetThreadUsage(std::thread::id threadID) const
    {
        if (!m_MemoryUsageByThread.contains(GetThreadIDHash(threadID))) { return 0; }
        return m_MemoryUsageByThread.at(GetThreadIDHash(threadID));
    }


    size_t MemoryMetrics::GetThreadPeakUsage(std::thread::id threadID) const
    {
        if (!m_PeakMemoryUsageByThread.contains(GetThreadIDHash(threadID))) { return 0; }
        return m_PeakMemoryUsageByThread.at(GetThreadIDHash(threadID));
    }


    size_t MemoryMetrics::GetThreadActiveAllocations(const std::thread::id threadID) const
    {
        if (!m_ActiveAllocationsByThread.contains(GetThreadIDHash(threadID))) { return 0; }
        return m_ActiveAllocationsByThread.at(GetThreadIDHash(threadID));
    }


    size_t MemoryMetrics::GetThreadTotalAllocations(const std::thread::id threadID) const
    {
        if (!m_TotalAllocationsByThread.contains(GetThreadIDHash(threadID))) { return 0; }
        return m_TotalAllocationsByThread.at(GetThreadIDHash(threadID));
    }

}
