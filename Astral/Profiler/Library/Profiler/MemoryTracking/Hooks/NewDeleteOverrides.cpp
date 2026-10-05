//
// Created by Andrew Fagan on 1/7/25.
//

#include "NewDeleteOverrides.h"

#include "../MemoryTracker.h"

void* operator new(std::size_t size)
{
    void* pointer = malloc(size);
    if (!pointer) throw std::bad_alloc();

    if (Astral::MemoryTracker::IsSingletonValid())
    {
        Astral::MemoryTracker::Get().AddAllocation(pointer, size, Astral::MemoryRegion::UNKNOWN, Astral::MemoryTrackerAllocatorType::NEW_OPERATOR);
    }
    return pointer;
}


void* operator new[](std::size_t size)
{
    void* pointer = malloc(size);
    if (!pointer) throw std::bad_alloc();

    if (Astral::MemoryTracker::IsSingletonValid())
    {
        Astral::MemoryTracker::Get().AddAllocation(pointer, size, Astral::MemoryRegion::UNKNOWN, Astral::MemoryTrackerAllocatorType::NEW_OPERATOR);
    }
    return pointer;
}


void operator delete(void* pointer) noexcept
{
    if (Astral::MemoryTracker::IsSingletonValid())
    {
        Astral::MemoryTracker::Get().RemoveAllocation(pointer);
    }
    free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    if (Astral::MemoryTracker::IsSingletonValid())
    {
        Astral::MemoryTracker::Get().RemoveAllocation(pointer);
    }
    free(pointer);
}
