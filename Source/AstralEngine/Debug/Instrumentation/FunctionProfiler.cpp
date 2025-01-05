//
// Created by Andrew Fagan on 1/5/25.
//

#include "FunctionProfiler.h"

#include "Debug/LogFiles/ConsoleLogFile.h"

namespace Debug {


    void FunctionProfiler::ProfileFunction(const std::function<void()>& functionToProfile, const char* functionName,
        const int numberOfIterations, bool suppressCout)
    {
        if (numberOfIterations <= 0) { return; }
        GetOriginalStream(); // Initializes cout stream storage variable if not initialized yet

        TRACE("Profiling Function: " << functionName << "!");

        // Redirect std::cout to a null stream
        if (suppressCout) { FunctionProfiler::SetCoutStreamToNullStream(); }
        ConsoleLogFile::GetInstance().Disable();

        const auto startTimeStamp = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numberOfIterations; i++)
        {
            functionToProfile();
        }

        const auto endTimeStamp = std::chrono::high_resolution_clock::now();

        if (suppressCout) { FunctionProfiler::RestoreCoutStream(); }
        ConsoleLogFile::GetInstance().Enable();

        const int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(startTimeStamp).time_since_epoch().count();
        const int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(endTimeStamp).time_since_epoch().count();
        const int64 duration = endTimePoint - startTimePoint;

        TRACE("Total Function Runtime: " << duration / 1000000.0f << " seconds");
        TRACE("Average Function Runtime: " << duration / numberOfIterations << " microseconds");
    }

    std::ofstream FunctionProfiler::m_NullStream = std::ofstream();

}