/**
* @file MemoryTracker.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "Debug/MemoryTracking/MemoryTracker.h"

#include "Debug/Utilities/Loggers.h"

namespace Astral {

    void MemoryTracker::Init()
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        m_IsTrackingEnabled = true;
        m_MemoryMetrics.Init();
        m_SceneMetricsExporter.InitExportFile();
#endif
    }


    void MemoryTracker::Shutdown()
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        m_MemoryMetrics.Shutdown();
        m_IsTrackingEnabled = false;
#endif
    }


    void MemoryTracker::BeginScene(const char* sceneName)
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        std::unique_lock lock(m_Mutex);
        bool successFlag = m_SceneMetricsExporter.BeginScene(sceneName);
        if (!successFlag) { LOG("Memory profiling scene \"" << sceneName << "\" failed to start!") }
#endif
    }


    bool MemoryTracker::IsSceneActive() const
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        std::shared_lock lock(m_Mutex);
        return m_SceneMetricsExporter.IsSceneActive();
#else
        return false;
#endif
    }


    void MemoryTracker::EndScene()
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        std::unique_lock lock(m_Mutex);
        m_SceneMetricsExporter.EndScene();
#endif
    }


    void MemoryTracker::AddAllocation(void* pointer, size_t size, MemoryRegion region, AllocatorType allocatorType)
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
        if (!IsTrackingEnabled()) { return; }

        std::unique_lock lock(m_Mutex);

        const AllocationData allocationData = {pointer, size, region, allocatorType, std::this_thread::get_id()};
        m_GlobalAllocationStorage.AddPointer(allocationData);

        m_MemoryMetrics.TrackAllocation(allocationData);

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.RecordMemoryMetrics(m_MemoryMetrics, allocationData);
        }
#endif
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
#ifndef ASTRAL_DISABLE_MEMORY_TRACKING
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
#endif
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


