/**
* @file MallocFreeOverrides.cpp
* @author Andrew Fagan
* @date 4/14/25
*/

#include "MallocFreeOverrides.h"

#include "MemoryTracker.h"
#include <cstdlib>

namespace Astral {

    void* TrackingMalloc(size_t size, Core::MemoryRegion region)
    {
        void* ptr = malloc(size);
        if (!ptr) [[unlikely]] { return nullptr; }
        Core::MemoryTracker::Get().AddAllocation(ptr, size, region, Core::AllocatorType::MALLOC);
        return ptr;
    }

    void* TrackingRealloc(void* ptr, size_t size, Core::MemoryRegion region)
    {
        void* newPtr = realloc(ptr, size);
        if (!newPtr) { return nullptr; }
        Core::MemoryTracker::Get().RemoveAllocation(ptr);
        Core::MemoryTracker::Get().AddAllocation(newPtr, size, region, Core::AllocatorType::MALLOC);
        return newPtr;
    }

    void TrackingFree(void* ptr)
    {
        Core::MemoryTracker::Get().RemoveAllocation(ptr);
        free(ptr);
    }

}
