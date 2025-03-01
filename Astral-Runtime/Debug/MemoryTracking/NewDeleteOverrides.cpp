//
// Created by Andrew Fagan on 1/7/25.
//

#include "NewDeleteOverrides.h"


#include <cstdlib>
#include <iostream>

#include "MemoryTracker.h"


void* operator new(std::size_t size)
{
    void* pointer = malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryTracker::Get().AddAllocation(pointer, size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::NEW_OPERATOR);
    return pointer;
}


void* operator new[](std::size_t size)
{
    void* pointer = malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryTracker::Get().AddAllocation(pointer, size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::NEW_OPERATOR);
    return pointer;
}


void operator delete(void* pointer) noexcept
{
    Core::MemoryTracker::Get().RemoveAllocation(pointer);
    free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    Core::MemoryTracker::Get().RemoveAllocation(pointer);
    free(pointer);
}
