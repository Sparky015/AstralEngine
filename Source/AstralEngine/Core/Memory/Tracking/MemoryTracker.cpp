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
        m_SceneMetricsExporter.InitExportFile();
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


    bool MemoryTracker::IsSceneActive() const
    {
        std::lock_guard lock(m_Mutex);
        return m_SceneMetricsExporter.IsSceneActive();
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

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics, allocationData);
        }
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
        std::lock_guard lock(m_Mutex);

        if (!m_GlobalAllocationStorage.IsPointerStored(pointer)) { return; }
        const AllocationData& allocationData = m_GlobalAllocationStorage.GetPointerData(pointer);
        m_MemoryMetrics.TrackDeallocation(allocationData);

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics, allocationData);
        }

        m_GlobalAllocationStorage.FreePointer(pointer);
    }

}
