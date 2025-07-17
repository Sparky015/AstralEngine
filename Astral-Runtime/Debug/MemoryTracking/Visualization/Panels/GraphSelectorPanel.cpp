/**
* @file GraphSelectorPanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "GraphSelectorPanel.h"

#include "imgui.h"

namespace Astral {

    MemoryGraphSelection ShowGraphSelectorPanel()
    {
        static MemoryGraphSelection currentSelection = MemoryGraphSelection::MEMORY_USAGE_GRAPH;

        ImGui::Begin("Memory Graph Selector");

        if (ImGui::RadioButton("Memory Usage", currentSelection == MemoryGraphSelection::MEMORY_USAGE_GRAPH))
        {
            currentSelection = MemoryGraphSelection::MEMORY_USAGE_GRAPH;
        }
        if (ImGui::RadioButton("Peak Memory Usage", currentSelection == MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH))
        {
            currentSelection = MemoryGraphSelection::PEAK_MEMORY_USAGE_GRAPH;
        }
        if (ImGui::RadioButton("Active Allocations", currentSelection == MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH))
        {
            currentSelection = MemoryGraphSelection::ACTIVE_ALLOCATION_GRAPH;
        }
        if (ImGui::RadioButton("Total Allocations", currentSelection == MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH))
        {
            currentSelection = MemoryGraphSelection::TOTAL_ALLOCATION_GRAPH;
        }

        ImGui::End();

        return currentSelection;
    }

}
