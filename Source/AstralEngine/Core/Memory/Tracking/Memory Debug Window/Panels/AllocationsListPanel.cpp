/**
* @file AllocationsListPanel.cpp
* @author Andrew Fagan
* @date 2/17/25
*/

#include "AllocationsListPanel.h"

#include <imgui/imgui.h>

namespace Core {

    // Getter for the old Combo() API: const char*[]
    static const char* SizeTItems_ArrayGetter(void* data, int idx)
    {
        static char buffer[200];
        AllocationDataSerializeable* items = (AllocationDataSerializeable*)data;
        snprintf(buffer, sizeof(buffer), "0x%zu | Size: %zu", items[idx].pointer, items[idx].size);
        return buffer;
    }


    size_t AllocationsListPanel(const SceneMetricsStorage& storage)
    {
        ImGui::Begin("Allocation List Panel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();


        static int item_current = 1;
        const std::vector<AllocationDataSerializeable>& allocationData = storage.GetAllocationDataOverTime();

        ImGui::Text("Snapshot Count: %zu", storage.GetSnapshotCount());

        if (ImGui::BeginListBox("List of Allocations m", ImGui::GetContentRegionAvail()))
        {
            ImGui::ListBox("List of Allocations", &item_current, SizeTItems_ArrayGetter, (void*)allocationData.data(), allocationData.size(), 15);
            ImGui::EndListBox();
        }

        ImGui::End();

        return item_current;
    }

}

