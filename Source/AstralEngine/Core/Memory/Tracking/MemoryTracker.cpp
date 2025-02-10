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

        const AllocationData allocationData = {pointer, size, region, allocatorType, std::this_thread::get_id()};
        m_GlobalAllocationStorage.AddPointer(allocationData);

        m_MemoryMetrics.TrackAllocation(allocationData);
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
        std::lock_guard lock(m_Mutex);

        if (!m_GlobalAllocationStorage.IsPointerStored(pointer)) { return; }
        const AllocationData& allocationData = m_GlobalAllocationStorage.GetPointerData(pointer);
        m_MemoryMetrics.TrackDeallocation(allocationData);
        m_GlobalAllocationStorage.FreePointer(pointer);
    }

}
