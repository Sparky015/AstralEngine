//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "pch.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

void* operator new(std::size_t size) {
    TRACE("Allocating " << size << " bytes")
    std::cout << "Allocating " << size << " bytes";
    void* p = std::malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept {
    std::cout << "Deallocating";
    std::free(p);
}