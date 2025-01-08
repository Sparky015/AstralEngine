//
// Created by Andrew Fagan on 1/7/25.
//

#include "MemoryComponents.h"

#include <imgui.h>

#include "Core/Memory/MemoryMetricsManager.h"

namespace Debug {

    void GlobalTotalAllocationsAndFrees()
    {
        Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Total Allocated Bytes: %llu", memoryMetricsManager.GetTotalAllocatedBytes());
        ImGui::Text("Total Freed Bytes: %llu", memoryMetricsManager.GetTotalFreedBytes());
    }

    void GlobalMemoryUsage()
    {
        Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        ImGui::Text("Memory Usage: %.2f MB", (memoryMetricsManager.GetTotalAllocatedBytes() - memoryMetricsManager.GetTotalFreedBytes()) / 1000000.0f);
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


