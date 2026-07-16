//
// Created by Andrew Fagan on 1/7/25.
//

#include "NewDeleteOverrides.h"


#include <cstdlib>
#include <iostream>

#include "MemoryTracker.h"
#include "Core/Memory/Allocators/TLSFAllocator.h"

static Astral::TLSFAllocator allocator = Astral::TLSFAllocator(4'000'000);
static std::mutex allocatorMutex = {};

void* operator new(std::size_t size)
{
    static Astral::TLSFAllocator allocator = Astral::TLSFAllocator(4'000'000);
    static std::mutex allocatorMutex = {};

    std::unique_lock lock (allocatorMutex);
    return allocator.Allocate(size);
    // void* pointer = malloc(size);
    // if (!pointer) throw std::bad_alloc();
    // Astral::MemoryTracker::Get().AddAllocation(pointer, size, Astral::MemoryRegion::UNKNOWN, Astral::MemoryTrackerAllocatorType::NEW_OPERATOR);
    // return pointer;
}


void* operator new[](std::size_t size)
{
    std::unique_lock lock (allocatorMutex);
    return allocator.Allocate(size);
    // void* pointer = malloc(size);
    // if (!pointer) throw std::bad_alloc();
    // Astral::MemoryTracker::Get().AddAllocation(pointer, size, Astral::MemoryRegion::UNKNOWN, Astral::MemoryTrackerAllocatorType::NEW_OPERATOR);
    // return pointer;
}


void operator delete(void* pointer) noexcept
{
    std::unique_lock lock (allocatorMutex);
    allocator.Free(pointer);
    // Astral::MemoryTracker::Get().RemoveAllocation(pointer);
    // free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    std::unique_lock lock (allocatorMutex);
    allocator.Free(pointer);
    // Astral::MemoryTracker::Get().RemoveAllocation(pointer);
    // free(pointer);
}
