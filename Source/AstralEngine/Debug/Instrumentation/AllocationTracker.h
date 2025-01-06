//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "../../pch.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

inline void* operator new(std::size_t size)
{
    // TODO: Add logging system
    void* pointer = std::malloc(size);
    if (!pointer) throw std::bad_alloc();
    return pointer;
}

inline void operator delete(void* pointer, std::size_t size) noexcept
{
    std::free(pointer);
}

// TODO: Write MemoryScopeProfiler that operators like the timer scope profiler
//      on construction, save the current total memory allocated and total freed memory and total number of allocations.
//      Then on destruction, mark the same things and measure the difference in data points.
//      This will show how much memory was allocated and freed and how many allocations occurred.