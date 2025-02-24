/**
* @file AllocationStacktracePanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"

namespace Core {

    void AllocationStackTracePanel(const SceneMetricsStorage& storage, size_t dataPointAddress);

}