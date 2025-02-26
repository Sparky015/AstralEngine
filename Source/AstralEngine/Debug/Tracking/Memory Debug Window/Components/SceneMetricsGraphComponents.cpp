/**
* @file SceneMetricsGraphs.cpp
* @author Andrew Fagan
* @date 2/9/2025
*/
#include "SceneMetricsGraphComponents.h"

#include "Debug/Macros/Asserts.h"
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


    void SetGraphAxisLimits(const SceneMetricsStorage& metricsStorage, const SceneMetricsStorage::DataPointStorage& dataStorage)
    {
        constexpr float GRAPH_MAX_LIMIT_MULTIPLIER = 1.1f;

        const auto maxElementIt = std::ranges::max_element(metricsStorage.GetAllocationTimes());
        double maxTime = maxElementIt == metricsStorage.GetAllocationTimes().end() ? 0 : *maxElementIt;

        ASSERT(dataStorage.size() == metricsStorage.GetSnapshotCount(), "Dataset size does not match the expected snapshot count!");

        GraphExtrema extrema = GetGraphExtrema(dataStorage);

        ImPlot::SetupAxesLimits(0, maxTime, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER, ImPlotCond_Once); // Only set on first frame
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, maxTime);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
        ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, maxTime);
        ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, extrema.min, extrema.max * GRAPH_MAX_LIMIT_MULTIPLIER);
    }


    void DrawSelectedDataPointLine(const SceneMetricsStorage& metricsStorage, size_t index)
    {
        ImPlot::PlotInfLines("Data Point Index", (ImU64*)&metricsStorage.GetAllocationTimes()[index], 1);
    }


    void GlobalMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        if (ImPlot::BeginPlot("Global Memory Usage"))
        {
            ImPlot::SetupAxes("Time (microseconds)", "Memory Usage (bytes)");

            SetGraphAxisLimits(storage, storage.GetGlobalMemoryUsageOverTime());
            DrawSelectedDataPointLine(storage, selectedPointIndex);

            ImPlot::PlotLine("Memory Usage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)storage.GetGlobalMemoryUsageOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        if (ImPlot::BeginPlot("Global Peak Memory Usage"))
        {
            ImPlot::SetupAxes("Allocation Number", "Peak Memory Usage");

            SetGraphAxisLimits(storage, storage.GetGlobalPeakMemoryUsageOverTime());
            DrawSelectedDataPointLine(storage, selectedPointIndex);

            ImPlot::PlotLine("Peak Memory Usage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)storage.GetGlobalPeakMemoryUsageOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalActiveAllocationGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        if (ImPlot::BeginPlot("Global Active Allocation Count"))
        {
            ImPlot::SetupAxes("Allocation Number", "Active Allocations");

            SetGraphAxisLimits(storage, storage.GetGlobalActiveAllocationsOverTime());
            DrawSelectedDataPointLine(storage, selectedPointIndex);

            ImPlot::PlotLine("Active Allocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)storage.GetGlobalActiveAllocationsOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void GlobalTotalAllocationGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        if (ImPlot::BeginPlot("Global Total Allocations Made"))
        {
            ImPlot::SetupAxes("Allocation Number", "Allocations Made");

            SetGraphAxisLimits(storage, storage.GetGlobalTotalAllocationsOverTime());
            DrawSelectedDataPointLine(storage, selectedPointIndex);

            ImPlot::PlotLine("Number of Allocations Made", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)storage.GetGlobalTotalAllocationsOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void AllocatorMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [allocatorType, allocationData] : storage.GetAllocatorMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(AllocatorTypeToString(allocatorType)))
            {
                ImPlot::SetupAxes("Time", "Memory Usage (bytes)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("AllocatorMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void AllocatorPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [allocatorType, allocationData] : storage.GetAllocatorPeakMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(AllocatorTypeToString(allocatorType)))
            {
                ImPlot::SetupAxes("Time", "Memory Usage (bytes)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("AllocatorPeakMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void AllocatorActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [allocatorType, allocationData] : storage.GetAllocatorActiveAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(AllocatorTypeToString(allocatorType)))
            {
                ImPlot::SetupAxes("Time", "Allocation Count");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("AllocatorActiveAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void AllocatorTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [allocatorType, allocationData] : storage.GetAllocatorTotalAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(AllocatorTypeToString(allocatorType)))
            {
                ImPlot::SetupAxes("Time", "Allocation Count");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("AllocatorTotalAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void RegionMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [region, allocationData] : storage.GetRegionMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(MemoryRegionToString(region)))
            {
                ImPlot::SetupAxes("Time", "Memory Usage (bytes)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("RegionMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void RegionPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [region, allocationData] : storage.GetRegionPeakMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(MemoryRegionToString(region)))
            {
                ImPlot::SetupAxes("Time", "Memory Usage (bytes)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("RegionPeakMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void RegionActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [region, allocationData] : storage.GetRegionActiveAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(MemoryRegionToString(region)))
            {
                ImPlot::SetupAxes("Time", "Allocation Count");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("RegionActiveAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void RegionTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [region, allocationData] : storage.GetRegionTotalAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(MemoryRegionToString(region)))
            {
                ImPlot::SetupAxes("Time", "Allocation Count");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("RegionTotalAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void ThreadMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [threadID, allocationData] : storage.GetThreadMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(std::format("{}", threadID).c_str()))
            {
                ImPlot::SetupAxes("Time", "Memory Usage (bytes)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void ThreadPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [threadID, allocationData] : storage.GetThreadPeakMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(std::format("{}", threadID).c_str()))
            {
                ImPlot::SetupAxes("Time", "Peak Memory Usage (byte)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadPeakMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void ThreadActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [threadID, allocationData] : storage.GetThreadActiveAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(std::format("{}", threadID).c_str()))
            {
                ImPlot::SetupAxes("Time", "Peak Memory Usage (byte)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadPeakMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }


    void ThreadTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [threadID, allocationData] : storage.GetThreadTotalAllocationsOverTimeIterable())
        {
            if (ImPlot::BeginPlot(std::format("{}", threadID).c_str()))
            {
                ImPlot::SetupAxes("Time", "Peak Memory Usage (byte)");

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadPeakMemoryUsage", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }

}

