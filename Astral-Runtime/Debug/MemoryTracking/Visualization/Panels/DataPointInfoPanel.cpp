/**
* @file DataPointInfoPanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "DataPointInfoPanel.h"

#include "imgui.h"

namespace Core {

    size_t* GetSelectedDataPointAddress()
    {
        return nullptr;
    }

    void ShowDataPointInfoPanel(const SceneMetricsStorage& storage, size_t selectedPointIndex)
    {
        ImGui::Begin("Data Point Info");

        if (selectedPointIndex == std::numeric_limits<size_t>::max())
        {
            ImGui::Text("No Data Point Selected");
        }
        else
        {
            ImGui::Text("Allocation Data:");
            ImGui::Spacing();
            ImGui::Text("Address: %lx", storage.GetAllocationDataOverTime()[selectedPointIndex].pointer);
            ImGui::Text("Size: %zu", storage.GetAllocationDataOverTime()[selectedPointIndex].size);
            ImGui::Text("Region: %s", MemoryRegionToString(storage.GetAllocationDataOverTime()[selectedPointIndex].region));
            ImGui::Text("Allocator: %s", AllocatorTypeToString(storage.GetAllocationDataOverTime()[selectedPointIndex].allocatorType));
            ImGui::Text("Thread ID Hash: %zu", storage.GetAllocationDataOverTime()[selectedPointIndex].threadIDHash);
            ImGui::Text("Time point (microseconds): %zu", storage.GetAllocationTimes()[selectedPointIndex]);
        }

        ImGui::End();
    }


}
