//
// Created by Andrew Fagan on 1/5/25.
//

#include "FunctionProfiler.h"

#include "Debug/LogFiles/ConsoleLogFile.h"

namespace Debug {


    void FunctionProfiler::ProfileFunction(const std::function<void()>& functionToProfile, const int numberOfIterations)
    {
        if (numberOfIterations <= 0) { return; }
        GetOriginalStream(); // Initializes cout stream storage variable if not initialized yet

        TRACE("Starting function profiler!");

        // Redirect std::cout to a null stream
        FunctionProfiler::SetCoutStreamToNullStream();
        ConsoleLogFile::GetInstance().Disable();

        const auto startTimeStamp = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numberOfIterations; i++)
        {
            functionToProfile();
        }

        const auto endTimeStamp = std::chrono::high_resolution_clock::now();

        // Restore the original buffer of std::cout
        FunctionProfiler::RestoreCoutStream();
        ConsoleLogFile::GetInstance().Enable();

        TRACE("Ending function profiler!");

        const int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(startTimeStamp).time_since_epoch().count();
        const int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(endTimeStamp).time_since_epoch().count();
        const int64 duration = endTimePoint - startTimePoint;

        LOG("Total Time: " << duration / 1000000.0f << " seconds");
        LOG("Average Time: " << duration / numberOfIterations << " microseconds");
    }

    std::ofstream FunctionProfiler::m_NullStream = std::ofstream();

}