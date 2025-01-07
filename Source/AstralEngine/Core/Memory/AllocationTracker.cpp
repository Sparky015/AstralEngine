//
// Created by Andrew Fagan on 1/7/25.
//

#include "AllocationTracker.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

#include "MemoryMetricsManager.h"


void* operator new(std::size_t size)
{
    Core::MemoryMetricsManager::Get().Allocate(size);
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    return pointer;
}


void* operator new[](std::size_t size)
{
    Core::MemoryMetricsManager::Get().Allocate(size);
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    return pointer;
}


void operator delete(void* pointer, std::size_t size) noexcept
{
    Core::MemoryMetricsManager::Get().Free(size);
    std::free(pointer);
}


void operator delete[](void* ptr, std::size_t size) noexcept
{
    Core::MemoryMetricsManager::Get().Free(size);
    std::free(ptr);
}


void operator delete(void* pointer) noexcept
{
    Core::MemoryMetricsManager::Get().Free(0);
    std::free(pointer);
}


void operator delete[](void* pointer) noexcept
{
    Core::MemoryMetricsManager::Get().Free(0);
    std::free(pointer);
}