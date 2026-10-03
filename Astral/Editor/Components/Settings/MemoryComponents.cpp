//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryComponents.h"

#include "Profiler/MemoryTracking/MemoryTracker.h"
#include "Core/PlatformAbstraction/CreateProcess.h"

#include <imgui.h>
#include <string_view>


namespace Astral {

    /**@brief Dynamically adjusts the unit label of the memory used based on the number of bytes being represented.
     * @param numberOfBytes The number of bytes that are being represented
     * @return A string representation of the memory size with the corresponding unit label
     * @note The unit label only goes up to TB
     * @warning The returned string_view is valid only until the next call to this function from the same thread.
     *          Do not store or cache the returned string_view. */
    std::string_view MemoryUnitLabelHelper(size_t numberOfBytes)
    {
        static constexpr int BUFFER_SIZE = 20;
        static constexpr size_t KB = 1000;
        static constexpr size_t MB = KB * 1000;
        static constexpr size_t GB = MB * 1000;
        static constexpr size_t TB = GB * 1000;
        thread_local char numberOfBytesString[BUFFER_SIZE] = {};

        if (numberOfBytes < KB) // show bytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%zu bytes", numberOfBytes);
        }
        else if (numberOfBytes < MB) // show kilobytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f KB", numberOfBytes / static_cast<float>(KB));
        }
        else if (numberOfBytes < GB) // show megabytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f MB", numberOfBytes / static_cast<float>(MB));
        }
        else if (numberOfBytes < TB) // show gigabytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f GB", numberOfBytes / static_cast<float>(GB));
        }
        else // show terabytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f TB", numberOfBytes / static_cast<float>(TB));
        }

        return numberOfBytesString;
    }


    void GlobalMemoryUsage()
    {
        size_t totalMemoryUsage = Astral::MemoryTracker::Get().GetTotalMemoryUsage();
        ImGui::Text("Memory Usage: %s", MemoryUnitLabelHelper(totalMemoryUsage).data());
    }


    void PeakMemoryUsage()
    {
        size_t peakMemoryUsage = Astral::MemoryTracker::Get().GetPeakMemoryUsage();
        ImGui::Text("Peak Memory Usage: %s", MemoryUnitLabelHelper(peakMemoryUsage).data());
    }


    void GlobalTotalAllocationsMade()
    {
        size_t totalAllocations = Astral::MemoryTracker::Get().GetTotalAllocations();
        ImGui::Text("Total Allocations Made: %zu", totalAllocations);
    }


    void GlobalActiveAllocations()
    {
        size_t totalActiveAllocations = Astral::MemoryTracker::Get().GetTotalActiveAllocations();
        ImGui::Text("Active Allocations: %zu", totalActiveAllocations);
    }


    void AllocationsInCurrentFrame()
    {
        Astral::FrameAllocationData frameAllocationData = Astral::MemoryTracker::Get().GetFrameAllocationData();
        ImGui::Text("Allocations made in current frame: %u", frameAllocationData.NumberOfAllocations);
    }


    void MemoryUsageByAllocator()
    {
        MemoryMetrics::AllocatorTypeMap memoryUsageByAllocatorIterable = Astral::MemoryTracker::Get().GetMemoryUsageByAllocatorIterable();

        if (ImGui::TreeNode("Memory Usage by Allocator"))
        {
            for (auto [allocatorType, size] : memoryUsageByAllocatorIterable)
            {
                ImGui::Text("%s: %s", Astral::AllocatorTypeToString(allocatorType), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void PeakMemoryUsageByAllocator()
    {
        MemoryMetrics::AllocatorTypeMap peakMemoryUsageByAllocatorIterable = Astral::MemoryTracker::Get().GetPeakMemoryUsageByAllocatorIterable();

        if (ImGui::TreeNode("Peak Memory Usage by Allocator"))
        {
            for (auto [allocatorType, size] : peakMemoryUsageByAllocatorIterable)
            {
                ImGui::Text("%s: %s", Astral::AllocatorTypeToString(allocatorType), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void TotalAllocationsMadeByAllocator()
    {
        MemoryMetrics::AllocatorTypeMap totalAllocationsByAllocatorIterable = Astral::MemoryTracker::Get().GetTotalAllocationsByAllocatorIterable();

        if (ImGui::TreeNode("Total Allocations Made by Allocator"))
        {
            for (auto [allocatorType, count] : totalAllocationsByAllocatorIterable)
            {
                ImGui::Text("%s: %zu", Astral::AllocatorTypeToString(allocatorType), count);
            }
            ImGui::TreePop();
        }
    }


    void ActiveAllocationsByAllocator()
    {
        MemoryMetrics::AllocatorTypeMap activeAllocationsByAllocatorIterable = Astral::MemoryTracker::Get().GetActiveAllocationsByAllocatorIterable();

        if (ImGui::TreeNode("Alive Allocations by Allocator"))
        {
            for (auto [allocatorType, count] : activeAllocationsByAllocatorIterable)
            {
                ImGui::Text("%s: %zu", Astral::AllocatorTypeToString(allocatorType), count);
            }
            ImGui::TreePop();
        }
    }


    void MemoryUsageByRegion()
    {
        MemoryMetrics::MemoryRegionMap memoryUsageByRegionIterable = Astral::MemoryTracker::Get().GetMemoryUsageByRegionIterable();

        if (ImGui::TreeNode("Memory Usage by Region"))
        {
            for (auto [region, size] : memoryUsageByRegionIterable)
            {
                ImGui::Text("%s: %s", Astral::MemoryRegionToString(region), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void PeakMemoryUsageByRegion()
    {
        MemoryMetrics::MemoryRegionMap peakMemoryUsageByRegionIterable = Astral::MemoryTracker::Get().GetPeakMemoryUsageByRegionIterable();

        if (ImGui::TreeNode("Peak Memory Usage by Region"))
        {
            for (auto [region, size] : peakMemoryUsageByRegionIterable)
            {
                ImGui::Text("%s: %s", Astral::MemoryRegionToString(region), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void TotalAllocationsMadeByRegion()
    {
        MemoryMetrics::MemoryRegionMap totalAllocationsByRegionIterable = Astral::MemoryTracker::Get().GetTotalAllocationsByRegionIterable();

        if (ImGui::TreeNode("Total Allocations Made by Region"))
        {
            for (auto [region, count] : totalAllocationsByRegionIterable)
            {
                ImGui::Text("%s: %zu", Astral::MemoryRegionToString(region), count);
            }
            ImGui::TreePop();
        }
    }


    void ActiveAllocationsByRegion()
    {
        MemoryMetrics::MemoryRegionMap activeAllocationsByRegionIterable = Astral::MemoryTracker::Get().GetActiveAllocationsByRegionIterable();

        if (ImGui::TreeNode("Alive Allocations by Region"))
        {
            for (auto [region, count] : activeAllocationsByRegionIterable)
            {
                ImGui::Text("%s: %zu", Astral::MemoryRegionToString(region), count);
            }
            ImGui::TreePop();
        }
    }


    void MemoryUsageByThread()
    {
        MemoryMetrics::ThreadMap memoryUsageByThreadIterable = Astral::MemoryTracker::Get().GetMemoryUsageByThreadIterable();

        if (ImGui::TreeNode("Memory Usage by Thread"))
        {

            for (auto [threadIDHash, size] : memoryUsageByThreadIterable)
            {
                size_t currentThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id()); // this is anticipated to be called from main
                if (currentThreadID == threadIDHash)
                {
                    ImGui::Text("Main Thread: %s", MemoryUnitLabelHelper(size).data());
                }
                else
                {
                    ImGui::Text("%zu: %s", threadIDHash, MemoryUnitLabelHelper(size).data());
                }
            }

            ImGui::TreePop();
        }
    }


    void PeakMemoryUsageByThread()
    {
        MemoryMetrics::ThreadMap peakMemoryUsageByThreadIterable = Astral::MemoryTracker::Get().GetPeakMemoryUsageByThreadIterable();

        if (ImGui::TreeNode("Peak Memory Usage by Thread"))
        {
            for (auto [threadIDHash, size] : peakMemoryUsageByThreadIterable)
            {
                size_t currentThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id()); // this is anticipated to be called from main
                if (currentThreadID == threadIDHash)
                {
                    ImGui::Text("Main Thread: %s", MemoryUnitLabelHelper(size).data());
                }
                else
                {
                    ImGui::Text("%zu: %s", threadIDHash, MemoryUnitLabelHelper(size).data());
                }
            }

            ImGui::TreePop();
        }
    }


    void TotalAllocationsMadeByThread()
    {
        MemoryMetrics::ThreadMap totalAllocationsByThreadIterable = Astral::MemoryTracker::Get().GetTotalAllocationsByThreadIterable();

        if (ImGui::TreeNode("Total Allocations Made by Thread"))
        {

            for (auto [threadIDHash, count] : totalAllocationsByThreadIterable)
            {
                size_t currentThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id()); // this is anticipated to be called from main
                if (currentThreadID == threadIDHash)
                {
                    ImGui::Text("Main Thread: %zu", count);
                }
                else
                {
                    ImGui::Text("%zu: %zu", threadIDHash, count);
                }
            }
            ImGui::TreePop();
        }
    }


    void ActiveAllocationsByThread()
    {
        MemoryMetrics::ThreadMap activeAllocationsByThreadIterable = Astral::MemoryTracker::Get().GetActiveAllocationsByThreadIterable();

        if (ImGui::TreeNode("Alive Allocations by Thread"))
        {

            for (auto [threadIDHash, count] : activeAllocationsByThreadIterable)
            {
                size_t currentThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id()); // this is anticipated to be called from main
                if (currentThreadID == threadIDHash)
                {
                    ImGui::Text("Main Thread: %zu", count);
                }
                else
                {
                    ImGui::Text("%zu: %zu", threadIDHash, count);
                }
            }

            ImGui::TreePop();
        }
    }


    void ManageMemoryProfilingScene()
    {
        Astral::MemoryTracker& tracker = Astral::MemoryTracker::Get();
        bool isSceneActive = tracker.IsSceneActive();

        ImGui::Checkbox("Enable Memory Profiling Scene", &isSceneActive);

        if (isSceneActive == tracker.IsSceneActive()) { return; }
        else if (isSceneActive)
        {
            tracker.BeginScene("DebugMenu");
        }
        else
        {
            tracker.EndScene();
        }
    }


    void OpenProfilerComponent()
    {
        if (ImGui::Button("Open Profiler"))
        {
            char* argv[] = {(char*)"AstralProfiler", nullptr};
            CreateChildProcess("./AstralProfiler", argv, nullptr);
        }
    }

}


