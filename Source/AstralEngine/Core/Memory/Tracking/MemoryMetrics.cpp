//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetrics.h"

namespace Core {

    void MemoryMetrics::TrackAllocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage += allocationInfo.size;
        m_PeakMemoryUsage = std::max(m_PeakMemoryUsage, m_TotalMemoryUsage);
        m_TotalActiveAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationInfo.size;
    }


    void MemoryMetrics::TrackDeallocation(const AllocationData& allocationInfo)
    {
        m_TotalMemoryUsage -= allocationInfo.size;
        m_TotalActiveAllocations--;
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
