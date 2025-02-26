//
// Created by Andrew Fagan on 1/5/25.
//

#pragma once

#include <fstream>

#include "../../pch.h"

namespace Debug {


    class FunctionProfiler
    {
    public:
        /**@brief Profiles a given function by calling it the given number of times. Defaults to 10,000 calls.
         * @param functionToProfile The function that is being profiled. Pass in a lambda that calls function if
         * function signature does not match with the parameter.
         * @param functionName The name of the function being profiled
         * @param numberOfIterations The amount of times that the function is called while profiling it
         * @param suppressCout Redirects cout stream to a nullstream to prevent spamming the console with messages */
        static void ProfileFunction(const std::function<void()>& functionToProfile, const char* functionName,
            const int numberOfIterations = 10000, bool suppressCout = true);

    private:

        /**@brief Suppresses output to cout when profiling a function */
        static inline void SetCoutStreamToNullStream() { std::cout.rdbuf(m_NullStream.rdbuf()); }

        /**@brief Restores output to cout when function profiler finishes */
        static inline void RestoreCoutStream() { std::cout.rdbuf(GetOriginalStream()); }

        /**@brief Stores the original cout buffer while avoiding initilzing a static varible */
        static inline std::streambuf* GetOriginalStream()
        {
            static std::streambuf* originalBuffer = std::cout.rdbuf();
            return originalBuffer;
        };

        static std::ofstream m_NullStream;
    };




}
