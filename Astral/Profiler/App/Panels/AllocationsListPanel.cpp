/**
* @file AllocationsListPanel.cpp
* @author Andrew Fagan
* @date 2/17/25
*/

#include "AllocationsListPanel.h"

#include "imgui.h"

namespace Astral {

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

    static AllocationsViewMode viewMode = AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER;

    static const char* AllocationDataArrayGetter(void* data, int idx)
    {
        static char buffer[200];

        if (viewMode == AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER)
        {
            AllocationDataSerializeable* items = (AllocationDataSerializeable*)data;
            snprintf(buffer, sizeof(buffer), "%p | Size: %zu bytes", items[idx].pointer, items[idx].size);
        }
        else if (viewMode == AllocationsViewMode::GROUPED_BY_SIZE)
        {
            std::pair<AllocationDataSerializeable, int>* items = (std::pair<AllocationDataSerializeable, int>*)data;
            snprintf(buffer, sizeof(buffer), "%p | Size: %zu bytes", items[idx].first.pointer, items[idx].first.size);
        }

        return buffer;
    }


    void AllocationsListPanel::Show(const SceneMetricsStorage& storage)
    {
        ImGui::Begin("Allocation List Panel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();


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
            const std::vector<AllocationDataSerializeable>& allocationData = storage.GetAllocationDataOverTime();
            m_SortedAllocationData.resize(allocationData.size());

            for (int i = 0; i < allocationData.size(); i++)
            {
                m_SortedAllocationData[i].first = allocationData[i];
                m_SortedAllocationData[i].second = i;
            }
            std::ranges::stable_sort(m_SortedAllocationData, [](const std::pair<AllocationDataSerializeable, int>& p1, const std::pair<AllocationDataSerializeable, int>& p2) {
                return p1.first.size < p2.first.size;
            });

            int tinyAllocationsEndIndex = -1;
            int smallAllocationsEndIndex = -1;
            int mediumAllocationsEndIndex = -1;


            for (int i = 0; i < m_SortedAllocationData.size(); i++)
            {
                if (m_SortedAllocationData[i].first.size > 128)
                {
                    tinyAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (tinyAllocationsEndIndex == -1) { tinyAllocationsEndIndex = m_SortedAllocationData.size() - 1; }

            for (int i = tinyAllocationsEndIndex; i < m_SortedAllocationData.size(); i++)
            {
                if (m_SortedAllocationData[i].first.size > 1024)
                {
                    smallAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (smallAllocationsEndIndex == -1) { smallAllocationsEndIndex = m_SortedAllocationData.size() - 1; }

            for (int i = smallAllocationsEndIndex; i < m_SortedAllocationData.size(); i++)
            {
                if (m_SortedAllocationData[i].first.size > 8192)
                {
                    mediumAllocationsEndIndex = i - 1;
                    break;
                }
            }
            if (mediumAllocationsEndIndex == -1) { mediumAllocationsEndIndex = m_SortedAllocationData.size() - 1; }

            int numberOfTinyAllocations = tinyAllocationsEndIndex + 1;
            static char tinyLabel[100] = "";
            snprintf(tinyLabel, 100, "Tiny (<= 128 bytes)  -  %d allocations/frees ##AllocationsListPanel", numberOfTinyAllocations);

            static char smallLabel[100] = "";
            int numberOfSmallAllocations = smallAllocationsEndIndex - tinyAllocationsEndIndex;
            snprintf(smallLabel, 100, "Small (129 - 1024 bytes)  -  %d allocations/frees ##AllocationsListPanel", numberOfSmallAllocations);

            static char mediumLabel[100] = "";
            int numberOfMediumAllocations = mediumAllocationsEndIndex - smallAllocationsEndIndex;
            snprintf(mediumLabel, 100, "Medium (1025 - 8192 bytes)  -  %d allocations/frees ##AllocationsListPanel", numberOfMediumAllocations);

            static char largeLabel[100] = "";
            int numberOfLargeAllocations = m_SortedAllocationData.size() - numberOfTinyAllocations - numberOfSmallAllocations - numberOfMediumAllocations;
            snprintf(largeLabel, 100, "Large (> 8 KB)  -  %d allocations/frees ##AllocationsListPanel", numberOfLargeAllocations);


            int tinySelectedIndex = -1;
            int smallSelectedIndex = -1;
            int mediumSelectedIndex = -1;
            int largeSelectedIndex = -1;
            size_t sortedSelectedPointSize = m_SortedAllocationData[m_SortedSelectedPointIndex].first.size;

            if (sortedSelectedPointSize <= 128)
            {
                tinySelectedIndex = m_SortedSelectedPointIndex;
            }
            else if (sortedSelectedPointSize <= 1024)
            {
                smallSelectedIndex = m_SortedSelectedPointIndex - numberOfTinyAllocations;
            }
            else if (sortedSelectedPointSize <= 8192)
            {
                mediumSelectedIndex = m_SortedSelectedPointIndex - numberOfTinyAllocations - numberOfSmallAllocations;
            }
            else
            {
                largeSelectedIndex = m_SortedSelectedPointIndex - numberOfTinyAllocations - numberOfSmallAllocations - numberOfMediumAllocations;
            }


            if (ImGui::TreeNode(tinyLabel))
            {
                if (ImGui::ListBox("##TinyAllocations", &tinySelectedIndex, AllocationDataArrayGetter, (void*)m_SortedAllocationData.data(), numberOfTinyAllocations, 10))
                {
                    m_SortedSelectedPointIndex = tinySelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(smallLabel))
            {
                int smallOffset = numberOfTinyAllocations;
                if (ImGui::ListBox("##SmallAllocations", &smallSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + smallOffset, numberOfSmallAllocations, 10))
                {
                    m_SortedSelectedPointIndex = smallOffset + smallSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(mediumLabel))
            {
                int mediumOffset = numberOfTinyAllocations + numberOfSmallAllocations;
                if (ImGui::ListBox("##MediumAllocations", &mediumSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + mediumOffset, numberOfMediumAllocations, 10))
                {
                    m_SortedSelectedPointIndex = mediumOffset + mediumSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(largeLabel))
            {
                int largeOffset = numberOfTinyAllocations + numberOfSmallAllocations + numberOfMediumAllocations;
                if (ImGui::ListBox("##LargeAllocations", &largeSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + largeOffset, numberOfLargeAllocations, 10))
                {
                    m_SortedSelectedPointIndex = largeOffset + largeSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
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

