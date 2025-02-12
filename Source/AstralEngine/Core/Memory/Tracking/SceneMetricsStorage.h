/**
* @file SceneMetricsStorage.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include <vector>

namespace Core {

    class SceneMetricsStorage
    {
    public:

    private:

        std::vector<size_t> m_PeakMemoryUsageOverTime;
        std::vector<size_t> m_TotalMemoryUsageOverTime;
        std::vector<size_t> m_TotalAllocationsOverTime;
        std::vector<size_t> m_TotalActiveAllocationsOverTime;
        // TODO: rest of the data in vectors
    };

}