//
// Created by Andrew Fagan on 1/2/25.
//

#include "Debug/Instrumentation/Public/ScopeProfiler.h"

#include "Debug/LogFiles/ProfilerLogFile.h"
#include "Debug/Tracking/Public/MemoryTracker.h"
#include "pch.h"

namespace Debug::Macros {

    macro_SCOPE_PROFILER::macro_SCOPE_PROFILER(const char* title) noexcept
        : m_title(title),
        m_StartTime(std::chrono::high_resolution_clock::now()),
        m_StartAllocationCount(Core::MemoryTracker::Get().GetMemoryMetrics().GetThreadTotalAllocations(std::this_thread::get_id()))
    {}


    macro_SCOPE_PROFILER::~macro_SCOPE_PROFILER() noexcept
    {
        m_EndTime = std::chrono::high_resolution_clock::now();
        m_EndAllocationCount = Core::MemoryTracker::Get().GetMemoryMetrics().GetThreadTotalAllocations(std::this_thread::get_id());

        int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
        int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTime).time_since_epoch().count();

        Debug::ProfilerLogFile::GetInstance().WriteProfile({m_title, startTimePoint, endTimePoint, m_EndAllocationCount - m_StartAllocationCount});
    }

}