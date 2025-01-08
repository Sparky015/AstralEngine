/**
* @file PointerAllocationSizeMap.h
* @author Andrew Fagan
* @date 1/7/2025
*/

#pragma once

#include <cstdlib>

#include "PointerAllocationSizeMapAllocator.h"


namespace Core {

    class PointerAllocationSizeMap
    {
    public:
        PointerAllocationSizeMap() = default;
        ~PointerAllocationSizeMap() = default;

        void AddPointer(void* pointer, size_t size);
        void FreePointer(void* pointer);
        size_t GetPointerSize(void* pointer);

    private:
        std::unordered_map<void*, size_t, std::hash<void*>, std::equal_to<void*>, PointerAllocationSizeMapAllocator<std::pair<void* const, size_t>>> m_Storage;
    };
}