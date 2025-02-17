/**
* @file SceneMetricsGraphs.cpp
* @author Andrew Fagan
* @date 2/9/2025
*/
#include "SceneMetricsGraphComponents.h"

#include "ImPlot/implot.h"

namespace Core {

    struct GraphExtrema
    {
        size_t max;
        size_t min;
    };

    GraphExtrema GetGraphExtrema(const SceneMetricsStorage::DataPointStorage& dataStorage)
    {
        const auto maxElementIt = std::ranges::max_element(dataStorage);
        const auto minElementIt = std::ranges::min_element(dataStorage);

        GraphExtrema extrema{};
        extrema.max = maxElementIt == dataStorage.end() ? 0 : *maxElementIt;
        extrema.min = minElementIt == dataStorage.end() ? 0 : *minElementIt;

        return extrema;
    }

    constexpr float GRAPH_MAX_LIMIT_MULTIPLIER = 1.1f;

    void GlobalMemoryUsageGraph(const SceneMetricsStorage& storage)
    {
        if (ImPlot::BeginPlot("Global Memory Usage"))
        {
            ImPlot::SetupAxes("Allocation Number", "Memory Usage");

            GraphExtrema extrema = GetGraphExtrema(storage.GetGlobalMemoryUsageOverTime());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);

            ImPlot::PlotLine("Memory Usage", (ImU64*)storage.GetGlobalMemoryUsageOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalPeakMemoryUsageGraph(const SceneMetricsStorage& storage)
    {
        if (ImPlot::BeginPlot("Global Peak Memory Usage"))
        {
            ImPlot::SetupAxes("Allocation Number", "Peak Memory Usage");

            GraphExtrema extrema = GetGraphExtrema(storage.GetGlobalPeakMemoryUsageOverTime());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);

            ImPlot::PlotLine("Peak Memory Usage", (ImU64*)storage.GetGlobalPeakMemoryUsageOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalActiveAllocationGraph(const SceneMetricsStorage& storage)
    {
        if (ImPlot::BeginPlot("Global Active Allocation Count"))
        {
            ImPlot::SetupAxes("Allocation Number", "Active Allocations");

            GraphExtrema extrema = GetGraphExtrema(storage.GetGlobalActiveAllocationsOverTime());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);

            ImPlot::PlotLine("Active Allocations", (ImU64*)storage.GetGlobalActiveAllocationsOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalTotalAllocationGraph(const SceneMetricsStorage& storage)
    {
        if (ImPlot::BeginPlot("Global Total Allocations Made"))
        {
            ImPlot::SetupAxes("Allocation Number", "Allocations Made");

            GraphExtrema extrema = GetGraphExtrema(storage.GetGlobalTotalAllocationsOverTime());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);

            ImPlot::PlotLine("Number of Allocations Made", (ImU64*)storage.GetGlobalTotalAllocationsOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }

}

