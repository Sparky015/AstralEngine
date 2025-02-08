//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetrics.h"

namespace Core {

    void MemoryMetrics::TrackAllocation(size_t allocationSize)
    {
        m_TotalMemoryUsage += allocationSize;
        m_PeakMemoryUsage = std::max(m_PeakMemoryUsage, m_TotalMemoryUsage);
        m_TotalActiveAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }


    void MemoryMetrics::TrackDeallocation(size_t deallocationSize)
    {
        m_TotalMemoryUsage -= deallocationSize;
        m_TotalActiveAllocations--;

        m_FrameAllocationData.NumberOfFrees++;
        m_FrameAllocationData.FreedBytes += deallocationSize;
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
