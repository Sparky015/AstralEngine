/**
* @file WindowTrackingAllocators.h
* @author Andrew Fagan
* @date 4/14/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Debug/MemoryTracking/AllocationData.h"

namespace Astral {

    void* WindowTrackingMalloc(size_t size, void* user);
    void* WindowTrackingRealloc(void* ptr, size_t size, void* user);
    void WindowTrackingFree(void* ptr, void* user);

}