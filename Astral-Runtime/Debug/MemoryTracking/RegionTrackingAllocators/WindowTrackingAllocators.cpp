/**
* @file WindowTrackingAllocators.cpp
* @author Andrew Fagan
* @date 4/14/25
*/

#include "WindowTrackingAllocators.h"

#include "../MemoryTracker.h"
#include <cstdlib>

namespace Astral {

    void* WindowTrackingMalloc(size_t size, void* user)
    {
        void* ptr = malloc(size);
        if (!ptr) [[unlikely]] { return nullptr; }
        Astral::MemoryTracker::Get().AddAllocation(ptr, size, Astral::MemoryRegion::WINDOW, Astral::AllocatorType::MALLOC);
        return ptr;
    }

    void* WindowTrackingRealloc(void* ptr, size_t size, void* user)
    {
        void* newPtr = realloc(ptr, size);
        if (!newPtr) { return nullptr; }
        Astral::MemoryTracker::Get().RemoveAllocation(ptr);
        Astral::MemoryTracker::Get().AddAllocation(newPtr, size, Astral::MemoryRegion::WINDOW, Astral::AllocatorType::MALLOC);
        return newPtr;
    }

    void WindowTrackingFree(void* ptr, void* user)
    {
        Astral::MemoryTracker::Get().RemoveAllocation(ptr);
        free(ptr);
    }

}
