/**
* @file AllocationsListPanel.cpp
* @author Andrew Fagan
* @date 2/17/25
*/

#include "AllocationsListPanel.h"

#include "imgui.h"

namespace Astral {

    static const char* AllocationDataArrayGetter(void* data, int idx)
    {
        static char buffer[200];
        AllocationDataSerializeable* items = (AllocationDataSerializeable*)data;
        snprintf(buffer, sizeof(buffer), "%p | Size: %zu bytes", items[idx].pointer, items[idx].size);
        return buffer;
    }

    enum class AllocationsViewMode
    {
        GROUPED_BY_ALLOCATION_ORDER,
        GROUPED_BY_SIZE
    };

    static std::string_view AllocationsViewModeToString(AllocationsViewMode allocationsViewMode)
    {
        switch (allocationsViewMode)
        {
            case AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER: return "Allocations in Order";
            case AllocationsViewMode::GROUPED_BY_SIZE:             return "Group by Size";
        }
        return "Undefined Case!";
    }


    void AllocationsListPanel::Show(const SceneMetricsStorage& storage)
    {
        ImGui::Begin("Allocation List Panel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        static AllocationsViewMode viewMode = AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER;

        ImGui::Text("Snapshot Count: %zu allocations/frees recorded", storage.GetSnapshotCount());

        ImGui::Spacing();
        ImGui::Text("View Mode:");
        ImGui::SameLine();

        if (ImGui::BeginCombo("##AllocationsListPanelViewSelector", AllocationsViewModeToString(viewMode).data()))
        {
            if (ImGui::Selectable("Allocations in Order"))
            {
                viewMode = AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER;
            }
            if (ImGui::Selectable("Group by Size"))
            {
                viewMode = AllocationsViewMode::GROUPED_BY_SIZE;
            }

            ImGui::EndCombo();
        }

        ImGui::Spacing();


        if (viewMode == AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER)
        {
            const std::vector<AllocationDataSerializeable>& allocationData = storage.GetAllocationDataOverTime();

            if (storage.GetSnapshotCount() == 0)
            {
                m_SelectedPointIndex = -1;
            }

            ImGui::PushItemWidth(-1);
            ImGui::ListBox("##List of Allocations", &m_SelectedPointIndex, AllocationDataArrayGetter, (void*)allocationData.data(), allocationData.size(), 30);
        }
        else if (viewMode == AllocationsViewMode::GROUPED_BY_SIZE)
        {
            std::vector<AllocationDataSerializeable> allocationData = storage.GetAllocationDataOverTime();
            std::ranges::sort(allocationData, {}, &AllocationDataSerializeable::size);

            int tinyAllocationsEndIndex = -1;
            int smallAllocationsEndIndex = -1;
            int mediumAllocationsEndIndex = -1;


            for (int i = 0; i < allocationData.size(); i++)
            {
                if (allocationData[i].size > 128)
                {
                    tinyAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (tinyAllocationsEndIndex == -1) { tinyAllocationsEndIndex = allocationData.size() - 1; }

            for (int i = tinyAllocationsEndIndex; i < allocationData.size(); i++)
            {
                if (allocationData[i].size > 1024)
                {
                    smallAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (smallAllocationsEndIndex == -1) { smallAllocationsEndIndex = allocationData.size() - 1; }

            for (int i = smallAllocationsEndIndex; i < allocationData.size(); i++)
            {
                if (allocationData[i].size > 8192)
                {
                    mediumAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (mediumAllocationsEndIndex == -1) { mediumAllocationsEndIndex = allocationData.size() - 1; }

            int numberOfTinyAllocations = tinyAllocationsEndIndex + 1;
            static char tinyLabel[100] = "";
            snprintf(tinyLabel, 100, "Tiny (<= 128 bytes)  -  %d allocations ##AllocationsListPanel", numberOfTinyAllocations);

            static char smallLabel[100] = "";
            int numberOfSmallAllocations = smallAllocationsEndIndex - tinyAllocationsEndIndex;
            snprintf(smallLabel, 100, "Small (129 - 1024 bytes)  -  %d allocations ##AllocationsListPanel", numberOfSmallAllocations);

            static char mediumLabel[100] = "";
            int numberOfMediumAllocations = mediumAllocationsEndIndex - smallAllocationsEndIndex;
            snprintf(mediumLabel, 100, "Medium (1025 - 8192 bytes)  -  %d allocations ##AllocationsListPanel", numberOfMediumAllocations);

            static char largeLabel[100] = "";
            int numberOfLargeAllocations = allocationData.size() - numberOfTinyAllocations - numberOfSmallAllocations - numberOfMediumAllocations;
            snprintf(largeLabel, 100, "Large (> 8 KB)  -  %d allocations ##AllocationsListPanel", numberOfLargeAllocations);


            if (ImGui::TreeNode(tinyLabel))
            {
                ImGui::ListBox("##TinyAllocations", &m_SelectedPointIndex, AllocationDataArrayGetter, (void*)allocationData.data(), numberOfTinyAllocations, 10);
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(smallLabel))
            {
                int smallOffset = numberOfTinyAllocations;
                ImGui::ListBox("##SmallAllocations", &m_SelectedPointIndex, AllocationDataArrayGetter, allocationData.data() + smallOffset, numberOfSmallAllocations, 10);
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(mediumLabel))
            {
                int mediumOffset = numberOfTinyAllocations + numberOfSmallAllocations;
                ImGui::ListBox("##MediumAllocations", &m_SelectedPointIndex, AllocationDataArrayGetter, allocationData.data() + mediumOffset, numberOfMediumAllocations, 10);
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(largeLabel))
            {
                int largeOffset = numberOfTinyAllocations + numberOfSmallAllocations + numberOfMediumAllocations;
                ImGui::ListBox("##LargeAllocations", &m_SelectedPointIndex, AllocationDataArrayGetter, allocationData.data() + largeOffset, numberOfLargeAllocations, 10);
                ImGui::Spacing();
                ImGui::TreePop();
            }

        }

        ImGui::End();
    }


    int AllocationsListPanel::GetSelectedPoint()
    {
        return m_SelectedPointIndex;
    }

}

