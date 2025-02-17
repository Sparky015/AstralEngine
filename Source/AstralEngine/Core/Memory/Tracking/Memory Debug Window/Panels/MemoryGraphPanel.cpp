/**
* @file MemoryGraphPanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "MemoryGraphPanel.h"

#include "../Components/SceneMetricsGraphComponents.h"

namespace Core {

    void ShowMemoryGraphPanel(const SceneMetricsStorage& storage, MemoryGraphSelection graphSelection)
    {
        ImGui::Begin("Memory Profiler Graph");

        switch (graphSelection)
        {
            case MemoryGraphSelection::MEMORY_USAGE_GRAPH:
                GlobalMemoryUsageGraph(storage);
                break;
            case MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH:
                GlobalPeakMemoryUsageGraph(storage);
                break;
            case MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH:
                GlobalActiveAllocationGraph(storage);
                break;
            case MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH:
                GlobalTotalAllocationGraph(storage);
                break;
            default: ERROR("MemoryGraphPanel: Invalid graph selection!");
        }

        ImGui::End();
    }

}
