//
// Created by Andrew Fagan on 1/7/25.
//

#include "AllocationTracker.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

#include "MemoryMetricsManager.h"


void* operator new(std::size_t size)
{
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryMetricsManager::Get().Allocate(pointer, size);
    return pointer;
}


void* operator new[](std::size_t size)
{
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryMetricsManager::Get().Allocate(pointer, size);
    return pointer;
}


void operator delete(void* pointer) noexcept
{
    Core::MemoryMetricsManager::Get().Free(pointer);
    std::free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    Core::MemoryMetricsManager::Get().Free(pointer);
    std::free(pointer);
}