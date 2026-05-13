/**
* @file AllocationsListPanel.h
* @author Andrew Fagan
* @date 2/17/25
*/

#pragma once

#include "Profiler/MemoryTracking/Serialization/SceneMetricsStorage.h"

namespace Astral {

    size_t AllocationsListPanel(const SceneMetricsStorage& storage);

}
