//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryMetricsManager.h"

namespace Core {

    // MemoryMetricsManager& MemoryMetricsManager::Get()
    // {
    //     static MemoryMetricsManager instance = MemoryMetricsManager();
    //     return instance;
    // }

    void MemoryMetricsManager::Allocate(void* pointer, size_t allocationSize)
    {
        m_PointerAllocationSizeMap.AddPointer(pointer, allocationSize);
        m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;
        m_TotalNumberOfAllocationsInFrame++;
    }

    void MemoryMetricsManager::Free(void* pointer)
    {
        size_t freeSize = m_PointerAllocationSizeMap.GetPointerSize(pointer);
        m_TotalFreedBytes += freeSize;
        m_TotalNumberOfFrees++;
        m_TotalNumberOfFreesInFrame++;

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
