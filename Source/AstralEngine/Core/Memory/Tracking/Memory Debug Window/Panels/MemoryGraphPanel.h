/**
* @file MemoryGraphPanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"
#include "GraphSelectorPanel.h"
#include "imgui.h"

namespace Core {

    void ShowMemoryGraphPanel(const SceneMetricsStorage& storage, MemoryGraphSelection graphSelection);

}
