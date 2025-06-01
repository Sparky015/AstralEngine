/**
* @file MemoryTracker.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "Debug/MemoryTracking/MemoryTracker.h"

#include "Debug/Utilities/Loggers.h"

namespace Core {

    void MemoryTracker::Init()
    {
        m_IsTrackingEnabled = true;
        m_MemoryMetrics.Init();
        m_SceneMetricsExporter.InitExportFile();
    }


    void MemoryTracker::Shutdown()
    {
        m_MemoryMetrics.Shutdown();
        m_IsTrackingEnabled = false;
    }


    void MemoryTracker::BeginScene(const char* sceneName)
    {
        std::unique_lock lock(m_Mutex);
        bool successFlag = m_SceneMetricsExporter.BeginScene(sceneName);
        if (!successFlag) { LOG("Memory profiling scene \"" << sceneName << "\" failed to start!") }
    }


    bool MemoryTracker::IsSceneActive() const
    {
        std::shared_lock lock(m_Mutex);
        return m_SceneMetricsExporter.IsSceneActive();
    }


    void MemoryTracker::EndScene()
    {
        std::unique_lock lock(m_Mutex);
        m_SceneMetricsExporter.EndScene();
    }


    void MemoryTracker::AddAllocation(void* pointer, size_t size, MemoryRegion region, AllocatorType allocatorType)
    {
        if (!IsTrackingEnabled()) { return; }

        std::unique_lock lock(m_Mutex);

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
        if (!IsTrackingEnabled()) { return; }

        std::unique_lock lock(m_Mutex);

        if (!m_GlobalAllocationStorage.IsPointerStored(pointer)) { return; }
        const AllocationData& allocationData = m_GlobalAllocationStorage.GetPointerData(pointer);
        m_MemoryMetrics.TrackDeallocation(allocationData);

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics, allocationData);
        }

        m_GlobalAllocationStorage.FreePointer(pointer);
    }


    void MemoryTracker::EnableTracking()
    {
        m_IsTrackingEnabled = true;
    }


    void MemoryTracker::DisableTracking()
    {
        m_IsTrackingEnabled = false;
    }


    bool MemoryTracker::IsTrackingEnabled()
    {
        return m_IsTrackingEnabled;
    }


    MemoryTracker::MemoryTracker() :
        m_IsTrackingEnabled(false)
    {
    }

    MemoryTracker::~MemoryTracker()
    {
        m_SceneMetricsExporter.EndScene();
        m_IsTrackingEnabled = false;
    }
}


