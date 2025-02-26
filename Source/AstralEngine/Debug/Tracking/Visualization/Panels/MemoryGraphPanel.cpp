/**
* @file MemoryGraphPanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "MemoryGraphPanel.h"

#include "Debug/Tracking/Visualization/Components/SceneMetricsGraphComponents.h"

namespace Core {

    void ShowMemoryGraphPanel(const SceneMetricsStorage& storage, MemoryMetricsDataScope metricsType, MemoryGraphSelection graphSelection, size_t
                              selectedDataPointIndex)
    {
        ImGui::Begin("Memory Profiler Graph");

        if (metricsType == MemoryMetricsDataScope::GLOBAL)
        {
            switch (graphSelection)
            {
                case MemoryGraphSelection::MEMORY_USAGE_GRAPH:
                    GlobalMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH:
                    GlobalPeakMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH:
                    GlobalActiveAllocationGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH:
                    GlobalTotalAllocationGraph(storage, selectedDataPointIndex);
                    break;
                default: ASTRAL_ERROR("MemoryGraphPanel: Invalid graph selection!");
            }
        }
        else if (metricsType == MemoryMetricsDataScope::ALLOCATOR)
        {
            switch (graphSelection)
            {
                case MemoryGraphSelection::MEMORY_USAGE_GRAPH:
                    AllocatorMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH:
                    AllocatorPeakMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH:
                    AllocatorActiveAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH:
                    AllocatorTotalAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                default: ASTRAL_ERROR("MemoryGraphPanel: Invalid graph selection!");
            }
        }
        else if (metricsType == MemoryMetricsDataScope::REGION)
        {
            switch (graphSelection)
            {
                case MemoryGraphSelection::MEMORY_USAGE_GRAPH:
                    RegionMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH:
                    RegionPeakMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH:
                    RegionActiveAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH:
                    RegionTotalAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                default: ASTRAL_ERROR("MemoryGraphPanel: Invalid graph selection!");
            }
        }
        else if (metricsType == MemoryMetricsDataScope::THREAD)
        {
            switch (graphSelection)
            {
                case MemoryGraphSelection::MEMORY_USAGE_GRAPH:
                    ThreadMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH:
                    ThreadPeakMemoryUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH:
                    ThreadActiveAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                case MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH:
                    ThreadTotalAllocationsUsageGraph(storage, selectedDataPointIndex);
                    break;
                default: ASTRAL_ERROR("MemoryGraphPanel: Invalid graph selection!");
            }
        }



        ImGui::End();
    }

}
