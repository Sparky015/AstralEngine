/**
* @file SceneMetricsGraphs.cpp
* @author Andrew Fagan
* @date 2/9/2025
*/
#include "SceneMetricsGraphComponents.h"

#include "Debug/Utilities/Asserts.h"
#include "ImPlot/implot.h"

namespace Astral {

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
        const auto maxElementIt = std::ranges::max_element(metricsStorage.GetAllocationTimes());
        double maxTime = maxElementIt == metricsStorage.GetAllocationTimes().end() ? 0 : *maxElementIt;

        ASSERT(dataStorage.size() == metricsStorage.GetSnapshotCount(), "Dataset size does not match the expected snapshot count!");

        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, maxTime);
    }


    int TimeAxisUnitFormatter(double microseconds, char* buff, int size, void* user_data)
    {
        if (microseconds == 0.0f) { return snprintf(buff, size, "0"); }

        if (std::abs(microseconds) < 1000.0)
        {
            return snprintf(buff, size, "%.*g us", 6, microseconds);
        }
        else if (std::abs(microseconds) < 1000000.0)
        {
            return snprintf(buff, size, "%.*g ms", 6, microseconds / 1000.0f);
        }
        else
        {
            return snprintf(buff, size, "%.*g s", 6, microseconds / 1000000.0f);
        }
    }

    int MemoryUsageAxisUnitFormatter(double bytesUsed, char* buff, int size, void* user_data)
    {
        if (std::abs(bytesUsed) < 1000.0)
        {
            return snprintf(buff, size, "%.0f bytes", bytesUsed);
        }
        else if (std::abs(bytesUsed) < 1000000.0)
        {
            return snprintf(buff, size, "%.2f KBs", bytesUsed / 1000.0f);
        }
        else if (std::abs(bytesUsed) < 1000000000.0)
        {
            return snprintf(buff, size, "%.2f MBs", bytesUsed / 1000000.0f);
        }
        else
        {
            return snprintf(buff, size, "%.2f GBs", bytesUsed / 1000000000.0f);
        }
    }

    int MemoryAllocationAxisUnitFormatter(double allocations, char* buff, int size, void* user_data)
    {
        if (std::abs(allocations) < 1000.0)
        {
            return snprintf(buff, size, "%.0f", allocations);
        }
        else if (std::abs(allocations) < 1000000.0)
        {
            return snprintf(buff, size, "%.3fK", allocations / 1000.0f);
        }
        else if (std::abs(allocations) < 1000000000.0)
        {
            return snprintf(buff, size, "%.3fM", allocations / 1000000.0f);
        }
        else
        {
            return snprintf(buff, size, "%.3fB", allocations / 1000000000.0f);
        }
    }


    void DrawSelectedDataPointLine(const SceneMetricsStorage& metricsStorage, size_t index)
    {
        ImPlot::PlotInfLines("Data Point Index", (ImU64*)&metricsStorage.GetAllocationTimes()[index], 1);
    }


    void GlobalMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        if (ImPlot::BeginPlot("Global Memory Usage"))
        {
            ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
            ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
            ImPlot::SetupAxes("Time", "Peak Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

            ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
            ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
            ImPlot::SetupAxes("Time", "Active Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

            ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
            ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

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
            ImPlot::SetupAxes("Time", "Total Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

            ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
            ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

            SetGraphAxisLimits(storage, storage.GetGlobalTotalAllocationsOverTime());
            DrawSelectedDataPointLine(storage, selectedPointIndex);

            ImPlot::PlotLine("Time", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)storage.GetGlobalTotalAllocationsOverTime().data(), storage.GetSnapshotCount());
            ImPlot::EndPlot();
        }
    }


    void AllocatorMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        for (auto [allocatorType, allocationData] : storage.GetAllocatorMemoryUsageOverTimeIterable())
        {
            if (ImPlot::BeginPlot(AllocatorTypeToString(allocatorType)))
            {
                ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Active Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Total Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Active Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Total Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Peak Memory Usage", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryUsageAxisUnitFormatter, nullptr);

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
                ImPlot::SetupAxes("Time", "Active Allocation Count", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadActiveAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
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
                ImPlot::SetupAxes("Time", "Total Allocations", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                ImPlot::SetupAxisFormat(ImAxis_X1, TimeAxisUnitFormatter, nullptr);
                ImPlot::SetupAxisFormat(ImAxis_Y1, MemoryAllocationAxisUnitFormatter, nullptr);

                SetGraphAxisLimits(storage, allocationData);
                DrawSelectedDataPointLine(storage, selectedPointIndex);

                ImPlot::PlotLine("ThreadTotalAllocations", (ImU64*)storage.GetAllocationTimes().data(), (ImU64*)allocationData.data(), storage.GetSnapshotCount());
                ImPlot::EndPlot();
            }
        }
    }

}

