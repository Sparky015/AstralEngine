//
// Created by Andrew Fagan on 1/5/25.
//

#pragma once

#include <fstream>

#include "pch.h"

namespace Debug {


    class FunctionProfiler
    {
    public:
        /**@brief Profiles a given function */
        static void ProfileFunction(const std::function<void()>& functionToProfile, const int numberOfIterations = 10000);

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
