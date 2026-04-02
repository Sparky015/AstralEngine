/**
* @file AllocationStacktracePanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Debug/MemoryTracking/Serialization/SceneMetricsStorage.h"

namespace Astral {

    void AllocationStackTracePanel(const SceneMetricsStorage& storage, size_t dataPointAddress);

}