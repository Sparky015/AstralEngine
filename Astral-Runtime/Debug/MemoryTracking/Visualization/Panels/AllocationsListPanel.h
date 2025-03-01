/**
* @file AllocationsListPanel.h
* @author Andrew Fagan
* @date 2/17/25
*/

#pragma once

#include "Debug/MemoryTracking/Serialization/SceneMetricsStorage.h"

namespace Core {

    size_t AllocationsListPanel(const SceneMetricsStorage& storage);

}
