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
        Core::MemoryTracker::Get().AddAllocation(ptr, size, Core::MemoryRegion::WINDOW, Core::AllocatorType::MALLOC);
        return ptr;
    }

    void* WindowTrackingRealloc(void* ptr, size_t size, void* user)
    {
        void* newPtr = realloc(ptr, size);
        if (!newPtr) { return nullptr; }
        Core::MemoryTracker::Get().RemoveAllocation(ptr);
        Core::MemoryTracker::Get().AddAllocation(newPtr, size, Core::MemoryRegion::WINDOW, Core::AllocatorType::MALLOC);
        return newPtr;
    }

    void WindowTrackingFree(void* ptr, void* user)
    {
        Core::MemoryTracker::Get().RemoveAllocation(ptr);
        free(ptr);
    }

}
