//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryComponents.h"

#include <imgui.h>

#include "Core/Memory/MemoryMetricsManager.h"

namespace Debug {

    std::string_view MemorySizeLabelHelper(size_t numberOfBytes)
    {
        static constexpr int BUFFER_SIZE = 20;
        thread_local char numberOfBytesString[BUFFER_SIZE] = {};
        memset(numberOfBytesString, 0, BUFFER_SIZE);

        if (numberOfBytes < 1000) // show bytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%zu bytes", numberOfBytes);
            return numberOfBytesString;
        }
        else if (numberOfBytes < 1000000) // show kilobytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f KB", numberOfBytes / 1000.0f);
            return numberOfBytesString;
        }
        else if (numberOfBytes < 1000000000) // show megabytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f MB", numberOfBytes / 1000000.0f);
            return numberOfBytesString;
        }
        else // show gigabytes
        {
            snprintf(numberOfBytesString, BUFFER_SIZE, "%.2f GB", numberOfBytes / 1000000.0f);
            return numberOfBytesString;
        }

    }

    void GlobalTotalAllocationsAndFrees()
    {
        Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Total Allocated Bytes: %llu", memoryMetricsManager.GetTotalAllocatedBytes());
        ImGui::Text("Total Freed Bytes: %llu", memoryMetricsManager.GetTotalFreedBytes());
    }

    void GlobalMemoryUsage()
    {
        Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Memory Usage: %s", MemorySizeLabelHelper(memoryMetricsManager.GetTotalAllocatedBytes() - memoryMetricsManager.GetTotalFreedBytes()).data());
    }

    void UnfreedMemoryFromCurrentFrame()
    {
        Core::FrameAllocationData frameAllocationData = Core::MemoryMetricsManager::Get().GetFrameAllocationData();
        ImGui::Text("Unfreed memory from current frame: %u bytes", (frameAllocationData.AllocatedBytes - frameAllocationData.FreedBytes));
    }

    void AllocationsAndFreesForCurrentFrame()
    {
        Core::FrameAllocationData frameAllocationData = Core::MemoryMetricsManager::Get().GetFrameAllocationData();
        ImGui::Text("Allocations in current frame: %u", frameAllocationData.NumberOfAllocations);
        ImGui::Text("Frees in current frame: %u", frameAllocationData.NumberOfFrees);
    }

}


