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
    void* p = std::malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

inline void operator delete(void* p) noexcept
{
    std::free(p);
}