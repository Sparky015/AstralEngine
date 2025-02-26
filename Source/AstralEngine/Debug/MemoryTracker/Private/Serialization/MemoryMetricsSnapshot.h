/**
* @file MemoryMetricsSnapshot.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Debug/Tracking/Private/MemoryMetrics.h"

namespace Core {

    struct MemoryMetricsSnapshot
    {
        MemoryMetricsSnapshot() = default;
        MemoryMetricsSnapshot(MemoryMetricsSnapshot&&) = default;
        MemoryMetrics memoryMetrics;
        float time{};

        MSGPACK_DEFINE(memoryMetrics, time);
    };

}