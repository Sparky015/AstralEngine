//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetricsManager.h"

namespace Core {

    void MemoryMetricsManager::Allocate(void* pointer, size_t allocationSize)
    {
        std::lock_guard lock(m_Mutex);
        m_PointerAllocationSizeMap.AddPointer(pointer, allocationSize);

        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;

        m_FrameAllocationData.NumberOfAllocations++;
        m_FrameAllocationData.AllocatedBytes += allocationSize;
    }

    void MemoryMetricsManager::Free(void* pointer)
    {
        std::lock_guard lock(m_Mutex);
        size_t freeSize = m_PointerAllocationSizeMap.GetPointerSize(pointer);

        m_TotalFreedBytes += freeSize;
        m_TotalNumberOfFrees++;

        m_FrameAllocationData.NumberOfFrees++;
        m_FrameAllocationData.FreedBytes += freeSize;

        m_PointerAllocationSizeMap.FreePointer(pointer);
    }

    void MemoryMetricsManager::Init()
    {
        m_NewFrameListener.StartListening();
    }

    void MemoryMetricsManager::Shutdown()
    {
        m_NewFrameListener.StopListening();
    }

}
