/**
* @file ViewConfigSelector.cpp
* @author Andrew Fagan 
* @date 10/26/26
*/

#include "ViewConfigSelector.h"

#include "Profiler/App/ProfilerApp.h"

#include "imgui.h"


namespace Astral {

    void ViewConfigSelector::Show(const SceneMetricsStorage& storage)
    {
        ImGui::Begin("View Filter Config");

        ShowThreadFilter(storage);
        ShowAllocatorFilter(storage);
        ShowMemoryRegionFilter(storage);

        ImGui::End();
    }


    void ViewConfigSelector::ShowThreadFilter(const SceneMetricsStorage& storage)
    {
        ProfilerViewConfig& config = ProfilerApp::Get().GetSceneDataCache().GetViewConfig();

        static char comboPreviewValue[256] = "";
        snprintf(comboPreviewValue, sizeof(comboPreviewValue), "%zu", config.GetThreadIDFilter());

        if (config.GetThreadIDFilter() == 0)
        {
            snprintf(comboPreviewValue, sizeof(comboPreviewValue), "All");
        }

        static char selectableValue[256] = "";

        if (ImGui::BeginCombo("Thread##ViewConfigSelector", comboPreviewValue))
        {
            if (ImGui::Selectable("All"))
            {
                config.SetThreadIDFilterIn(0);
            }

            for (auto [threadID, allocationData] : storage.GetThreadMemoryUsageOverTimeIterable())
            {
                snprintf(selectableValue, sizeof(selectableValue), "%zu", threadID);
                if (ImGui::Selectable(selectableValue))
                {
                    config.SetThreadIDFilterIn(threadID);
                }
            }

            ImGui::EndCombo();
        }
    }


    void ViewConfigSelector::ShowAllocatorFilter(const SceneMetricsStorage& storage)
    {
        ProfilerViewConfig& config = ProfilerApp::Get().GetSceneDataCache().GetViewConfig();

        static char comboPreviewValue[256] = "";
        snprintf(comboPreviewValue, sizeof(comboPreviewValue), "%s", AllocatorTypeToString(config.GetAllocatorFilter()));

        if (config.GetAllocatorFilter() == MemoryTrackerAllocatorType::ALLOCATOR_TYPE_END)
        {
            snprintf(comboPreviewValue, sizeof(comboPreviewValue), "All");
        }

        if (ImGui::BeginCombo("Allocator##ViewConfigSelector", comboPreviewValue))
        {
            if (ImGui::Selectable("All"))
            {
                config.SetAllocatorFilterIn(MemoryTrackerAllocatorType::ALLOCATOR_TYPE_END);
            }

            for (auto [allocatorType, allocationData] : storage.GetAllocatorMemoryUsageOverTimeIterable())
            {
                if (ImGui::Selectable(AllocatorTypeToString(allocatorType)))
                {
                    config.SetAllocatorFilterIn(allocatorType);
                }
            }

            ImGui::EndCombo();
        }
    }


    void ViewConfigSelector::ShowMemoryRegionFilter(const SceneMetricsStorage& storage)
    {
        ProfilerViewConfig& config = ProfilerApp::Get().GetSceneDataCache().GetViewConfig();

        static char comboPreviewValue[256] = "";
        snprintf(comboPreviewValue, sizeof(comboPreviewValue), "%s", MemoryRegionToString(config.GetRegionFilter()));

        if (config.GetRegionFilter() == MemoryRegion::MEMORY_REGION_END)
        {
            snprintf(comboPreviewValue, sizeof(comboPreviewValue), "All");
        }

        if (ImGui::BeginCombo("Memory Region##ViewConfigSelector", comboPreviewValue))
        {
            if (ImGui::Selectable("All"))
            {
                config.SetRegionFilterIn(MemoryRegion::MEMORY_REGION_END);
            }

            for (auto [memoryRegion, allocationData] : storage.GetRegionMemoryUsageOverTimeIterable())
            {
                if (ImGui::Selectable(MemoryRegionToString(memoryRegion)))
                {
                    config.SetRegionFilterIn(memoryRegion);
                }
            }

            ImGui::EndCombo();
        }
    }

}
