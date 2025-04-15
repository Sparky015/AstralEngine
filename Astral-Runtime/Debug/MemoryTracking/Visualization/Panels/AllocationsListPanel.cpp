/**
* @file AllocationsListPanel.cpp
* @author Andrew Fagan
* @date 2/17/25
*/

#include "AllocationsListPanel.h"

#include "imgui.h"

namespace Core {

    static const char* AllocationDataArrayGetter(void* data, int idx)
    {
        static char buffer[200];
        AllocationDataSerializeable* items = (AllocationDataSerializeable*)data;
        snprintf(buffer, sizeof(buffer), "0x%lx | Size: %zu bytes", items[idx].pointer, items[idx].size);
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

        ImGui::PushItemWidth(-1);
        ImGui::ListBox("##List of Allocations", &item_current, AllocationDataArrayGetter, (void*)allocationData.data(), allocationData.size(), 30);

        ImGui::End();

        return item_current;
    }

}

