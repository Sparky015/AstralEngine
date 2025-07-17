/**
* @file MemoryGraphPanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Debug/MemoryTracking/Serialization/SceneMetricsStorage.h"
#include "GraphSelectorPanel.h"
#include "imgui.h"

namespace Astral {

    enum class MemoryMetricsDataScope : uint8
    {
        GLOBAL,
        ALLOCATOR,
        REGION,
        THREAD
    };

    void ShowMemoryGraphPanel(const SceneMetricsStorage& storage, MemoryMetricsDataScope metricsType, MemoryGraphSelection graphSelection, size_t
                              selectedDataPointIndex);

}
