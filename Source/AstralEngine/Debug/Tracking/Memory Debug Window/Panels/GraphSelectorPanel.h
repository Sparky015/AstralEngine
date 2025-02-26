/**
* @file GraphSelectorPanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"

namespace Core {

    enum class MemoryGraphSelection : uint8
    {
        MEMORY_USAGE_GRAPH,
        PEAK_MEMORY_USAGE_GRAPH,
        TOTAL_ALLOCATION_GRAPH,
        ACTIVE_ALLOCATION_GRAPH
    };

    MemoryGraphSelection ShowGraphSelectorPanel();

}