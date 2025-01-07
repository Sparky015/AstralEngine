//
// Created by Andrew Fagan on 1/7/25.
//

#include "CoreComponents.h"

#include <imgui.h>

#include "Core/Memory/MemoryMetricsManager.h"

namespace Debug {

    void MemoryTrackerComponent()
    {
        Core::MemoryMetricsManager& memoryMetricsManager = Core::MemoryMetricsManager::Get();
        // ImGui::Text("Total Allocated Bytes: %llu", memoryMetricsManager.GetTotalAllocatedBytes());
        // ImGui::Text("Total Freed Bytes: %llu", memoryMetricsManager.GetTotalFreedBytes());
        ImGui::Text("Total Number of Allocations: %u", memoryMetricsManager.GetTotalNumberOfAllocations());
        ImGui::Text("Total Number of Frees: %u", memoryMetricsManager.GetTotalNumberOfFrees());
        // ImGui::Text("Memory Usage: %.2f MB", (memoryMetricsManager.GetTotalAllocatedBytes() - memoryMetricsManager.GetTotalFreedBytes()) / 1000000.0f);
        ImGui::Text("Alive Allocations: %u", (memoryMetricsManager.GetTotalNumberOfAllocations() - memoryMetricsManager.GetTotalNumberOfFrees()));
        ImGui::Text("Alive Allocations in Frame: %u", (memoryMetricsManager.GetAliveAllocationsInFrame()));

    }

}


