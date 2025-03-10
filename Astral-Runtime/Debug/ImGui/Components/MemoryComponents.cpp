//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryComponents.h"

#include <imgui.h>
#include <string_view>

#include "Debug/MemoryTracking/MemoryTracker.h"

namespace Debug {

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
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();
        ImGui::Text("Memory Usage: %s", MemoryUnitLabelHelper(memoryMetrics.GetTotalMemoryUsage()).data());
    }


    void PeakMemoryUsage()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();
        ImGui::Text("Peak Memory Usage: %s", MemoryUnitLabelHelper(memoryMetrics.GetPeakMemoryUsage()).data());
    }


    void GlobalTotalAllocationsMade()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();
        ImGui::Text("Total Allocations Made: %zu", memoryMetrics.GetTotalAllocations());
    }


    void GlobalActiveAllocations()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();
        ImGui::Text("Active Allocations: %zu", memoryMetrics.GetTotalActiveAllocations());
    }


    void AllocationsInCurrentFrame()
    {
        const Core::FrameAllocationData frameAllocationData = Core::MemoryTracker::Get().GetMemoryMetrics().GetFrameAllocationData();
        ImGui::Text("Allocations in current frame: %u", frameAllocationData.NumberOfAllocations);
    }


    void MemoryUsageByAllocator()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Memory Usage by Allocator"))
        {
            for (auto [allocatorType, size] : memoryMetrics.GetMemoryUsageByAllocatorIterable())
            {
                ImGui::Text("%s: %s", Core::AllocatorTypeToString(allocatorType), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void PeakMemoryUsageByAllocator()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Peak Memory Usage by Allocator"))
        {
            for (auto [allocatorType, size] : memoryMetrics.GetPeakMemoryUsageByAllocatorIterable())
            {
                ImGui::Text("%s: %s", Core::AllocatorTypeToString(allocatorType), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void TotalAllocationsMadeByAllocator()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Total Allocations Made by Allocator"))
        {
            for (auto [allocatorType, count] : memoryMetrics.GetTotalAllocationsByAllocatorIterable())
            {
                ImGui::Text("%s: %zu", Core::AllocatorTypeToString(allocatorType), count);
            }
            ImGui::TreePop();
        }
    }


    void ActiveAllocationsByAllocator()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Alive Allocations by Allocator"))
        {
            for (auto [allocatorType, count] : memoryMetrics.GetActiveAllocationsByAllocatorIterable())
            {
                ImGui::Text("%s: %zu", Core::AllocatorTypeToString(allocatorType), count);
            }
            ImGui::TreePop();
        }
    }


    void MemoryUsageByRegion()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Memory Usage by Region"))
        {
            for (auto [region, size] : memoryMetrics.GetMemoryUsageByRegionIterable())
            {
                ImGui::Text("%s: %s", Core::MemoryRegionToString(region), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void PeakMemoryUsageByRegion()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Peak Memory Usage by Region"))
        {
            for (auto [region, size] : memoryMetrics.GetPeakMemoryUsageByRegionIterable())
            {
                ImGui::Text("%s: %s", Core::MemoryRegionToString(region), MemoryUnitLabelHelper(size).data());
            }
            ImGui::TreePop();
        }
    }


    void TotalAllocationsMadeByRegion()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Total Allocations Made by Region"))
        {
            for (auto [region, count] : memoryMetrics.GetTotalAllocationsByRegionIterable())
            {
                ImGui::Text("%s: %zu", Core::MemoryRegionToString(region), count);
            }
            ImGui::TreePop();
        }
    }


    void ActiveAllocationsByRegion()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Alive Allocations by Region"))
        {
            for (auto [region, count] : memoryMetrics.GetActiveAllocationsByRegionIterable())
            {
                ImGui::Text("%s: %zu", Core::MemoryRegionToString(region), count);
            }
            ImGui::TreePop();
        }
    }


    void MemoryUsageByThread()
    {
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Memory Usage by Thread"))
        {

            for (auto [threadIDHash, size] : memoryMetrics.GetMemoryUsageByThreadIterable())
            {
                if (memoryMetrics.GetThreadIDHash(std::this_thread::get_id()) == threadIDHash)
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
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Peak Memory Usage by Thread"))
        {

            for (auto [threadIDHash, size] : memoryMetrics.GetPeakMemoryUsageByThreadIterable())
            {
                if (memoryMetrics.GetThreadIDHash(std::this_thread::get_id()) == threadIDHash)
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
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Total Allocations Made by Thread"))
        {

            for (auto [threadIDHash, count] : memoryMetrics.GetTotalAllocationsByThreadIterable())
            {
                if (memoryMetrics.GetThreadIDHash(std::this_thread::get_id()) == threadIDHash)
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
        const Core::MemoryMetrics& memoryMetrics = Core::MemoryTracker::Get().GetMemoryMetrics();

        if (ImGui::TreeNode("Alive Allocations by Thread"))
        {

            for (auto [threadIDHash, count] : memoryMetrics.GetActiveAllocationsByThreadIterable())
            {
                if (memoryMetrics.GetThreadIDHash(std::this_thread::get_id()) == threadIDHash)
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
        Core::MemoryTracker& tracker = Core::MemoryTracker::Get();
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

}


