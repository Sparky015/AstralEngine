/**
* @file MemoryTracker.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "MemoryTracker.h"

#include "Debug/Macros/Loggers.h"

namespace Core {

    void MemoryTracker::Init()
    {
        m_MemoryMetrics.Init();
    }

    void MemoryTracker::Shutdown()
    {
        m_MemoryMetrics.Shutdown();
    }

    void MemoryTracker::BeginScene(const char* sceneName)
    {
        std::lock_guard lock(m_Mutex);
        bool successFlag = m_SceneMetricsExporter.BeginScene(sceneName);
        if (!successFlag) { LOG("Memory profiling scene \"" << sceneName << "\" failed to start!") }
    }


    void MemoryTracker::EndScene()
    {
        std::lock_guard lock(m_Mutex);
        m_SceneMetricsExporter.EndScene();
    }


    void MemoryTracker::AddAllocation(void* pointer, size_t size, MemoryRegion region, AllocatorType allocatorType)
    {
        std::lock_guard lock(m_Mutex);

        const AllocationData allocationData = {pointer, size, region, allocatorType, std::this_thread::get_id()};
        m_GlobalAllocationStorage.AddPointer(allocationData);

        m_MemoryMetrics.TrackAllocation(allocationData);

        m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics);
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
        std::lock_guard lock(m_Mutex);

        if (!m_GlobalAllocationStorage.IsPointerStored(pointer)) { return; }
        const AllocationData& allocationData = m_GlobalAllocationStorage.GetPointerData(pointer);
        m_MemoryMetrics.TrackDeallocation(allocationData);
        m_GlobalAllocationStorage.FreePointer(pointer);

        m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics);
    }

}
