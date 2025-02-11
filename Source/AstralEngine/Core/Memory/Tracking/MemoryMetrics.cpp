//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetrics.h"

namespace Core {

    MemoryMetrics::MemoryMetrics() :
        m_PeakMemoryUsage(0),
        m_TotalMemoryUsage(0),
        m_TotalActiveAllocations(0),
        m_NewFrameEventListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }}
    {
        // Initialize unordered_maps keys with all enum values
    }


    void MemoryMetrics::TrackAllocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage += allocationInfo.size;
        m_PeakMemoryUsage = std::max(m_PeakMemoryUsage, m_TotalMemoryUsage);
        m_TotalActiveAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationInfo.size;

        m_MemoryUsageByAllocator[allocationInfo.allocatorType] += allocationInfo.size;
        m_MemoryUsageByRegion[allocationInfo.region] += allocationInfo.size;
        m_MemoryUsageByThread[allocationInfo.threadID] += allocationInfo.size;

        m_ActiveAllocationsByAllocator[allocationInfo.allocatorType]++;
        m_ActiveAllocationsByRegion[allocationInfo.region]++;
        m_ActiveAllocationsByThread[allocationInfo.threadID]++;

        m_TotalAllocationsByAllocator[allocationInfo.allocatorType]++;
        m_TotalAllocationsByRegion[allocationInfo.region]++;
        m_TotalAllocationsByThread[allocationInfo.threadID]++;

        m_PeakMemoryUsageByAllocator[allocationInfo.allocatorType] = std::max(
            m_MemoryUsageByAllocator[allocationInfo.allocatorType],
            m_PeakMemoryUsageByAllocator[allocationInfo.allocatorType]);

        m_PeakMemoryUsageByRegion[allocationInfo.region] = std::max(
            m_MemoryUsageByRegion[allocationInfo.region],
            m_PeakMemoryUsageByRegion[allocationInfo.region]);


        m_PeakMemoryUsageByThread[allocationInfo.threadID] = std::max(
            m_MemoryUsageByThread[allocationInfo.threadID],
            m_PeakMemoryUsageByThread[allocationInfo.threadID]);
    }


    void MemoryMetrics::TrackDeallocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage -= allocationInfo.size;
        m_TotalActiveAllocations--;

        m_MemoryUsageByAllocator[allocationInfo.allocatorType] -= allocationInfo.size;
        m_MemoryUsageByRegion[allocationInfo.region] -= allocationInfo.size;
        m_MemoryUsageByThread[allocationInfo.threadID] -= allocationInfo.size;

        m_ActiveAllocationsByAllocator[allocationInfo.allocatorType]--;
        m_ActiveAllocationsByRegion[allocationInfo.region]--;
        m_ActiveAllocationsByThread[allocationInfo.threadID]--;
    }


    void MemoryMetrics::Init()
    {
        m_NewFrameEventListener.StartListening();
    }


    void MemoryMetrics::Shutdown()
    {
        m_NewFrameEventListener.StopListening();
    }


}
