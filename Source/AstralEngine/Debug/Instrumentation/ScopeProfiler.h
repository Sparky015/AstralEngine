//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once

#include <chrono>

/** Profiles a scope and outputs the time to the console. */
#ifndef TURN_OFF_PROFILER_MACRO
#define PROFILE_SCOPE(title) Debug::Macros::macro_SCOPE_PROFILER localScopedProfiler = Debug::Macros::macro_SCOPE_PROFILER(title);
#else
#define PROFILE_SCOPE()
#endif


namespace Debug::Macros {

    /** Profiles a scope and outputs the time to the console. */
    class macro_SCOPE_PROFILER {
    public:
        explicit macro_SCOPE_PROFILER(const char* title) noexcept;
        ~macro_SCOPE_PROFILER() noexcept;
    private:
        const char* m_title;
        const std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    };

}