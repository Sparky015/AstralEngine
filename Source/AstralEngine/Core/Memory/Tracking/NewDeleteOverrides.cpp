//
// Created by Andrew Fagan on 1/7/25.
//

#include "NewDeleteOverrides.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

#include "MemoryMetrics.h"

void* operator new(std::size_t size)
{
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryMetrics::Get().TrackAllocation(pointer, size);
    return pointer;
}


void* operator new[](std::size_t size)
{
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryMetrics::Get().TrackAllocation(pointer, size);
    return pointer;
}


void operator delete(void* pointer) noexcept
{
    Core::MemoryMetrics::Get().TrackDeallocation(pointer);
    std::free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    Core::MemoryMetrics::Get().TrackDeallocation(pointer);
    std::free(pointer);
}