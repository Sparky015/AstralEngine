//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryComponents.h"

#include <imgui.h>

#include "Core/Memory/MemoryMetricsManager.h"

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

    void GlobalTotalAllocationsAndFrees()
    {
        const Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Total Allocated Bytes: %llu", memoryMetricsManager.GetTotalAllocatedBytes());
        ImGui::Text("Total Freed Bytes: %llu", memoryMetricsManager.GetTotalFreedBytes());
    }

    void GlobalMemoryUsage()
    {
        const Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Memory Usage: %s", MemoryUnitLabelHelper(memoryMetricsManager.GetTotalAllocatedBytes() - memoryMetricsManager.GetTotalFreedBytes()).data());
    }

    void UnfreedMemoryFromCurrentFrame()
    {
        const Core::FrameAllocationData frameAllocationData = Core::MemoryMetricsManager::Get().GetFrameAllocationData();
        ImGui::Text("Unfreed memory from current frame: %u bytes", (frameAllocationData.AllocatedBytes - frameAllocationData.FreedBytes));
    }

    void AllocationsAndFreesForCurrentFrame()
    {
        const Core::FrameAllocationData frameAllocationData = Core::MemoryMetricsManager::Get().GetFrameAllocationData();
        ImGui::Text("Allocations in current frame: %u", frameAllocationData.NumberOfAllocations);
        ImGui::Text("Frees in current frame: %u", frameAllocationData.NumberOfFrees);
    }

}


