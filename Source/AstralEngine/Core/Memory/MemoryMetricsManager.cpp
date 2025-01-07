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

    void MemoryMetricsManager::Allocate(size_t allocationSize)
    {
        // m_TotalAllocatedBytes += allocationSize;
        m_TotalNumberOfAllocations++;
        m_TotalNumberOfAllocationsPerFrame++;
    }

    void MemoryMetricsManager::Free(size_t freeSize)
    {
        // m_TotalFreedBytes += freeSize;
        m_TotalNumberOfFrees++;
        m_TotalNumberOfFreesPerFrame++;
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
