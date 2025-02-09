/**
* @file MemoryTracker.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "MemoryTracker.h"

namespace Core {

    MemoryTracker::MemoryTracker()
    {
    }


    MemoryTracker::~MemoryTracker()
    {
    }


    void MemoryTracker::Init()
    {
        m_MemoryMetrics.Init();
    }

    void MemoryTracker::Shutdown()
    {
        m_MemoryMetrics.Shutdown();
    }

    void MemoryTracker::BeginScene() const
    {

    }


    void MemoryTracker::EndScene() const
    {

    }


    void MemoryTracker::AddAllocation(void* pointer, size_t size, MemoryRegion region, AllocatorType allocatorType)
    {
        std::lock_guard lock(m_Mutex);
        m_GlobalAllocationStorage.AddPointer(pointer, size);
        m_MemoryMetrics.TrackAllocation(size);
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
        std::lock_guard lock(m_Mutex);
        size_t deallocationSize = m_GlobalAllocationStorage.GetPointerSize(pointer);
        m_GlobalAllocationStorage.FreePointer(pointer);
        m_MemoryMetrics.TrackDeallocation(deallocationSize);
    }

}
