//
// Created by Andrew Fagan on 1/2/25.
//

#include "ScopeProfiler.h"

#include "Debug/LogFiles/ProfilerLogFile.h"
#include "pch.h"


namespace Debug::Macros {

    macro_SCOPE_PROFILER::macro_SCOPE_PROFILER(const char* title) noexcept
        : m_title(title),
        m_startTime(std::chrono::high_resolution_clock::now())
    {}


    macro_SCOPE_PROFILER::~macro_SCOPE_PROFILER() noexcept
    {
        m_endTime = std::chrono::high_resolution_clock::now();

        int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
        int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTime).time_since_epoch().count();

        Debug::ProfilerLogFile::GetInstance().WriteProfile({m_title, startTimePoint, endTimePoint});
    }

}