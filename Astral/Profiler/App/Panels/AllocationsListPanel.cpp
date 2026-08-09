/**
* @file AllocationsListPanel.cpp
* @author Andrew Fagan
* @date 2/17/25
*/

#include "AllocationsListPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "ImPlot/implot.h"

namespace Astral {

    Vec2 AllocationsListPanel::m_FilterInPlotLimits = {};
    size_t AllocationsListPanel::m_FilterInThreadID = 0;
    MemoryRegion AllocationsListPanel::m_FilterInMemoryRegion = MemoryRegion::MEMORY_REGION_END;
    MemoryTrackerAllocatorType AllocationsListPanel::m_FilterInAllocatorType = MemoryTrackerAllocatorType::ALLOCATOR_TYPE_END;

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

    static const SceneMetricsStorage* localScopeStorage = nullptr;
    const char* AllocationsListPanel::AllocationDataArrayGetter(void* data, int idx)
    {
        static char buffer[200];

        int storageIndex = -1;
        if (viewMode == AllocationsViewMode::GROUPED_BY_ALLOCATION_ORDER)
        {
            AllocationDataSerializeable* items = (AllocationDataSerializeable*)data;
            snprintf(buffer, sizeof(buffer), "%p | Size: %zu bytes", items[idx].pointer, items[idx].size);
            storageIndex = idx;
        }
        else if (viewMode == AllocationsViewMode::GROUPED_BY_SIZE)
        {
            std::pair<AllocationDataSerializeable, int>* items = (std::pair<AllocationDataSerializeable, int>*)data;
            snprintf(buffer, sizeof(buffer), "%p | Size: %zu bytes", items[idx].first.pointer, items[idx].first.size);
            storageIndex = items[idx].second;
        }

        // Filters
        if (m_FilterInPlotLimits != Vec2(0) && !(m_FilterInPlotLimits.x <= localScopeStorage->GetAllocationTimes()[storageIndex] && localScopeStorage->GetAllocationTimes()[storageIndex] <= m_FilterInPlotLimits.y))
        {
            return nullptr;
        }
        if (m_FilterInThreadID != 0 && localScopeStorage->GetAllocationDataOverTime()[storageIndex].threadIDHash != m_FilterInThreadID)
        {
            return nullptr;
        }
        if (m_FilterInMemoryRegion != MemoryRegion::MEMORY_REGION_END && localScopeStorage->GetAllocationDataOverTime()[storageIndex].region != m_FilterInMemoryRegion)
        {
            return nullptr;
        }
        if (m_FilterInAllocatorType != MemoryTrackerAllocatorType::ALLOCATOR_TYPE_END && localScopeStorage->GetAllocationDataOverTime()[storageIndex].allocatorType != m_FilterInAllocatorType)
        {
            return nullptr;
        }

        return buffer;
    }


    void AllocationsListPanel::Show(const SceneMetricsStorage& storage)
    {
        localScopeStorage = &storage; // to read storage variable from the implot getter

        ImGui::Begin("Allocation List Panel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();


        ImGui::Text("Snapshot Count: %zu total allocations/frees recorded", storage.GetSnapshotCount());

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

            int filteredOutCount = 0;
            for (int i = 0; i < allocationData.size(); i++)
            {
                if (AllocationDataArrayGetter((void*)allocationData.data(), i) == nullptr)
                {
                    filteredOutCount++;
                }
            }

            ImGui::Text("Filtered-In Allocations: %zu", allocationData.size() - filteredOutCount);
            ImGui::PushItemWidth(-1);
            ImGuiCustomListBox("##List of Allocations", &m_SelectedPointIndex, AllocationDataArrayGetter, (void*)allocationData.data(), allocationData.size(), 30);
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
            int numberOfSmallAllocations = smallAllocationsEndIndex - tinyAllocationsEndIndex;
            int numberOfMediumAllocations = mediumAllocationsEndIndex - smallAllocationsEndIndex;
            int numberOfLargeAllocations = m_SortedAllocationData.size() - numberOfTinyAllocations - numberOfSmallAllocations - numberOfMediumAllocations;

            int smallDataOffset = numberOfTinyAllocations;
            int mediumDataOffset = numberOfTinyAllocations + numberOfSmallAllocations;
            int largeDataOffset = numberOfTinyAllocations + numberOfSmallAllocations + numberOfMediumAllocations;

            int tinyAllocationsFilteredIn = 0;
            int smallAllocationsFilteredIn = 0;
            int mediumAllocationsFilteredIn = 0;
            int largeAllocationsFilteredIn = 0;

            std::array<int, 4> allocationNumbers = {numberOfTinyAllocations, numberOfSmallAllocations, numberOfMediumAllocations, numberOfLargeAllocations};
            std::array<int*, 4> outFilterInAllocationCount = {&tinyAllocationsFilteredIn, &smallAllocationsFilteredIn, &mediumAllocationsFilteredIn, &largeAllocationsFilteredIn};
            std::array<int, 4> dataOffsets = {0, smallDataOffset, mediumDataOffset, largeDataOffset};

            // Count number of allocations filtered out among tiny, small, medium, and large allocations
            for (int i = 0; i < 4; i++)
            {
                int filteredOutCount = 0;

                for (int j = 0; j < allocationNumbers[i]; j++)
                {
                    if (AllocationDataArrayGetter((void*)(m_SortedAllocationData.data() + dataOffsets[i]), j) == nullptr)
                    {
                        filteredOutCount++;
                    }
                }
                *outFilterInAllocationCount[i] = allocationNumbers[i] - filteredOutCount;
            }

            static char tinyLabel[150] = "";
            static char smallLabel[150] = "";
            static char mediumLabel[150] = "";
            static char largeLabel[150] = "";

            snprintf(tinyLabel, sizeof(tinyLabel),     "Tiny (<= 128 bytes)         -  %d allocations/frees filtered in   /  %d total allocations/frees  ##TinyAllocationsListPanel", tinyAllocationsFilteredIn, numberOfTinyAllocations);
            snprintf(smallLabel, sizeof(smallLabel),   "Small (129 - 1024 bytes)    -  %d allocations/frees filtered in   /  %d total allocations/frees  ##SmallAllocationsListPanel", smallAllocationsFilteredIn, numberOfSmallAllocations);
            snprintf(mediumLabel, sizeof(mediumLabel), "Medium (1025 - 8192 bytes)  -  %d allocations/frees filtered in   /  %d total allocations/frees  ##MediumAllocationsListPanel", mediumAllocationsFilteredIn, numberOfMediumAllocations);
            snprintf(largeLabel, sizeof(largeLabel),   "Large (> 8 KB)              -  %d allocations/frees filtered in   /  %d total allocations/frees  ##LargeAllocationsListPanel", largeAllocationsFilteredIn, numberOfLargeAllocations);


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
                smallSelectedIndex = m_SortedSelectedPointIndex - smallDataOffset;
            }
            else if (sortedSelectedPointSize <= 8192)
            {
                mediumSelectedIndex = m_SortedSelectedPointIndex - mediumDataOffset;
            }
            else
            {
                largeSelectedIndex = m_SortedSelectedPointIndex - largeDataOffset;
            }


            if (ImGui::TreeNode(tinyLabel))
            {
                if (ImGuiCustomListBox("##TinyAllocations", &tinySelectedIndex, AllocationDataArrayGetter, (void*)m_SortedAllocationData.data(), numberOfTinyAllocations, 10))
                {
                    m_SortedSelectedPointIndex = tinySelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(smallLabel))
            {
                int filteredOutCount = 0;
                for (int i = 0; i < numberOfSmallAllocations; i++)
                {
                    if (AllocationDataArrayGetter((void*)(m_SortedAllocationData.data() + smallDataOffset), i) == nullptr)
                    {
                        filteredOutCount++;
                    }
                }
                ImGui::Text("Filtered-In Allocations: %d", numberOfSmallAllocations - filteredOutCount);

                if (ImGuiCustomListBox("##SmallAllocations", &smallSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + smallDataOffset, numberOfSmallAllocations, 10))
                {
                    m_SortedSelectedPointIndex = smallDataOffset + smallSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(mediumLabel))
            {
                int filteredOutCount = 0;
                for (int i = 0; i < numberOfMediumAllocations; i++)
                {
                    if (AllocationDataArrayGetter((void*)(m_SortedAllocationData.data() + mediumDataOffset), i) == nullptr)
                    {
                        filteredOutCount++;
                    }
                }
                ImGui::Text("Filtered-In Allocations: %d", numberOfMediumAllocations - filteredOutCount);

                if (ImGuiCustomListBox("##MediumAllocations", &mediumSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + mediumDataOffset, numberOfMediumAllocations, 10))
                {
                    m_SortedSelectedPointIndex = mediumDataOffset + mediumSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(largeLabel))
            {
                int filteredOutCount = 0;
                for (int i = 0; i < numberOfLargeAllocations; i++)
                {
                    if (AllocationDataArrayGetter((void*)(m_SortedAllocationData.data() + largeDataOffset), i) == nullptr)
                    {
                        filteredOutCount++;
                    }
                }
                ImGui::Text("Filtered-In Allocations: %d", numberOfLargeAllocations - filteredOutCount);

                if (ImGuiCustomListBox("##LargeAllocations", &largeSelectedIndex, AllocationDataArrayGetter, m_SortedAllocationData.data() + largeDataOffset, numberOfLargeAllocations, 10))
                {
                    m_SortedSelectedPointIndex = largeDataOffset + largeSelectedIndex;
                    m_SelectedPointIndex = m_SortedAllocationData[m_SortedSelectedPointIndex].second;
                }
                ImGui::Spacing();
                ImGui::TreePop();
            }

        }

        ImGui::End();

        localScopeStorage = nullptr; // set to nullptr to prevent dangling reference
    }


    int AllocationsListPanel::GetSelectedPoint()
    {
        return m_SelectedPointIndex;
    }


    bool AllocationsListPanel::ImGuiCustomListBox(const char* label, int* current_item,
                                                  const char*(* getter)(void* user_data, int idx), void* user_data,
                                                  int items_count, int height_in_items)
    {
        ImGuiContext& g = *ImGui::GetCurrentContext();

        // Calculate size from "height_in_items"
        if (height_in_items < 0)
        {
            height_in_items = ImMin(items_count, 7);
        }
        float height_in_items_f = height_in_items + 0.25f;
        ImVec2 size(0.0f, ImTrunc(ImGui::GetTextLineHeightWithSpacing() * height_in_items_f + g.Style.FramePadding.y * 2.0f));

        if (!ImGui::BeginListBox(label, size))
        {
            return false;
        }


        bool value_changed = false;

        for (int i = 0; i < items_count; i++)
        {
            const char* item_text = getter(user_data, i);
            if (item_text == NULL)
            {
                continue;
            }

            ImGui::PushID(i);
            const bool item_selected = (i == *current_item);
            if (ImGui::Selectable(item_text, item_selected))
            {
                *current_item = i;
                value_changed = true;
            }
            if (item_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
            ImGui::PopID();
        }

        ImGui::EndListBox();

        if (value_changed)
        {
            ImGui::MarkItemEdited(g.LastItemData.ID);
        }

        return value_changed;
    }


    void AllocationsListPanel::SetListPlotLimitsFilterIn(Vec2 plotLimits)
    {
        m_FilterInPlotLimits = plotLimits;
    }


    void AllocationsListPanel::SetListThreadIDFilterIn(size_t hashedThreadID)
    {
        m_FilterInThreadID = hashedThreadID;
    }


    void AllocationsListPanel::SetListRegionFilterIn(MemoryRegion memoryRegion)
    {
        m_FilterInMemoryRegion = memoryRegion;
    }


    void AllocationsListPanel::SetListAllocatorFilterIn(MemoryTrackerAllocatorType allocatorType)
    {
        m_FilterInAllocatorType = allocatorType;
    }

}

