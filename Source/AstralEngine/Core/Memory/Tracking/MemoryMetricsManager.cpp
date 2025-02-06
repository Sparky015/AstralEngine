//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetricsManager.h"

namespace Core {

    void MemoryMetricsManager::TrackAllocation(void* allocatedPointer, size_t allocationSize)
    {
        std::lock_guard lock(m_Mutex);

        if (!allocatedPointer) { return; }

        m_PointerAllocationSizeMap.AddPointer(allocatedPointer, allocationSize);

        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }

    void MemoryMetricsManager::TrackAllocation(size_t allocationSize)
    {
        std::lock_guard lock(m_Mutex);

        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }

    void MemoryMetricsManager::TrackDeallocation(void* pointerToBeFreed)
    {
        std::lock_guard lock(m_Mutex);

        if (!pointerToBeFreed) { return; }

        const size_t freeSize = m_PointerAllocationSizeMap.GetPointerSize(pointerToBeFreed);

        m_TotalFreedBytes += freeSize;
        m_TotalNumberOfFrees++;

        m_FrameAllocationData.NumberOfFrees++;
        m_FrameAllocationData.FreedBytes += freeSize;

        m_PointerAllocationSizeMap.FreePointer(pointerToBeFreed);
    }

    void MemoryMetricsManager::TrackDeallocation(size_t deallocationSize)
    {
        m_TotalFreedBytes += deallocationSize;
        m_TotalNumberOfFrees++;

        m_FrameAllocationData.NumberOfFrees++;
        m_FrameAllocationData.FreedBytes += deallocationSize;
    }

    void MemoryMetricsManager::Init()
    {
        m_NewFrameEventListener.StartListening();
    }

    void MemoryMetricsManager::Shutdown()
    {
        m_NewFrameEventListener.StopListening();
    }




}
