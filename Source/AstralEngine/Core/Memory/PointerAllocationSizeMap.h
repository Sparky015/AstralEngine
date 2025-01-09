/**
* @file PointerAllocationSizeMap.h
* @author Andrew Fagan
* @date 1/7/2025
*/

#pragma once

#include <cstdlib>

#include "NoTrackingAllocator.h"

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
        std::unordered_map<const void*, size_t, std::hash<const void*>, std::equal_to<const void*>, NoTrackingAllocator<std::pair<const void* const, size_t>>> m_Storage;
    };
}