//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetrics.h"

namespace Core {

    void MemoryMetrics::TrackAllocation(void* allocatedPointer, size_t allocationSize)
    {
        std::lock_guard lock(m_Mutex);

        if (!allocatedPointer) { return; }

        m_GlobalAllocationStorage.AddPointer(allocatedPointer, allocationSize);

        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }


    void MemoryMetrics::TrackAllocation(size_t allocationSize)
    {
        std::lock_guard lock(m_Mutex);

        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }


    void MemoryMetrics::TrackDeallocation(void* pointerToBeFreed)
    {
        std::lock_guard lock(m_Mutex);

        if (!pointerToBeFreed) { return; }

        const size_t freeSize = m_GlobalAllocationStorage.GetPointerSize(pointerToBeFreed);

        m_TotalFreedBytes += freeSize;
        m_TotalNumberOfFrees++;

        m_FrameAllocationData.NumberOfFrees++;
        m_FrameAllocationData.FreedBytes += freeSize;

        m_GlobalAllocationStorage.FreePointer(pointerToBeFreed);
    }


    void MemoryMetrics::TrackDeallocation(size_t deallocationSize)
    {
        m_TotalFreedBytes += deallocationSize;
        m_TotalNumberOfFrees++;

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
