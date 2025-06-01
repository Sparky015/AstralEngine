/**
* @file MallocFreeOverrides.h
* @author Andrew Fagan
* @date 4/14/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Debug/MemoryTracking/AllocationData.h"

namespace Astral {

    void* TrackingMalloc(size_t size, MemoryRegion region = UNKNOWN);
    void* TrackingRealloc(void* ptr, size_t size, MemoryRegion region = UNKNOWN);
    void TrackingFree(void* ptr);

}
