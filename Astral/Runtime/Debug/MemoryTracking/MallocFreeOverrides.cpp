/**
* @file MallocFreeOverrides.cpp
* @author Andrew Fagan
* @date 4/14/25
*/

#include "MallocFreeOverrides.h"

#include "MemoryTracker.h"
#include <cstdlib>

namespace Astral {

    void* TrackingMalloc(size_t size, Astral::MemoryRegion region)
    {
        void* ptr = malloc(size);
        if (!ptr) [[unlikely]] { return nullptr; }
        Astral::MemoryTracker::Get().AddAllocation(ptr, size, region, Astral::AllocatorType::MALLOC);
        return ptr;
    }

    void* TrackingRealloc(void* ptr, size_t size, Astral::MemoryRegion region)
    {
        void* newPtr = realloc(ptr, size);
        if (!newPtr) { return nullptr; }
        Astral::MemoryTracker::Get().RemoveAllocation(ptr);
        Astral::MemoryTracker::Get().AddAllocation(newPtr, size, region, Astral::AllocatorType::MALLOC);
        return newPtr;
    }

    void TrackingFree(void* ptr)
    {
        Astral::MemoryTracker::Get().RemoveAllocation(ptr);
        free(ptr);
    }

}
