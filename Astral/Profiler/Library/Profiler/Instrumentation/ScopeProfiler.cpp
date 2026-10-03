//
// Created by Andrew Fagan on 1/2/25.
//

#include "ScopeProfiler.h"

#include "ProfilerLogFile.h"
#include "Profiler/MemoryTracking/MemoryTracker.h"

namespace Astral::Macros {

    macro_SCOPE_PROFILER::macro_SCOPE_PROFILER(const char* title) noexcept
        : m_title(title),
        m_StartTime(),
        m_StartAllocationCount(0),
        m_EndAllocationCount(0)
    {
#ifdef ASTRAL_SCOPE_MEMORY_ALLOCATION_PROFILER_ENABLED
        Astral::MemoryTracker::Get().ProcessDeferredOperationsBuffer(); // Process deferred allocations/frees to get updated count of total thread allocations
        m_StartAllocationCount = Astral::MemoryTracker::Get().GetThreadTotalAllocations(std::this_thread::get_id());
#endif

        m_StartTime = std::chrono::high_resolution_clock::now();
    }


    macro_SCOPE_PROFILER::~macro_SCOPE_PROFILER() noexcept
    {
        m_EndTime = std::chrono::high_resolution_clock::now();

#ifdef ASTRAL_SCOPE_MEMORY_ALLOCATION_PROFILER_ENABLED
        Astral::MemoryTracker::Get().ProcessDeferredOperationsBuffer(); // Process deferred allocations/frees to get updated count of total thread allocations
        m_EndAllocationCount = Astral::MemoryTracker::Get().GetThreadTotalAllocations(std::this_thread::get_id());
#endif

        int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
        int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTime).time_since_epoch().count();

        Astral::ProfilerLogFile::GetInstance().WriteProfile({m_title, startTimePoint, endTimePoint, m_EndAllocationCount - m_StartAllocationCount});
    }

}