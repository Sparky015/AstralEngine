/**
* @file PointerAllocationSizeMap.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "PointerAllocationSizeMap.h"

namespace Core {


    void PointerAllocationSizeMap::AddPointer(const void* pointer, const size_t size)
    {
        if (!pointer) { return; }
        m_Storage[pointer] = size;
    }


    void PointerAllocationSizeMap::FreePointer(const void* pointer)
    {
        if (!pointer) { return; }

        const auto it = m_Storage.find(pointer);
        if (it == m_Storage.end()) { return; }
        m_Storage.erase(pointer);
    }


    size_t PointerAllocationSizeMap::GetPointerSize(const void* pointer)
    {
        if (!pointer) { return 0; }

        const auto it = m_Storage.find(pointer);
        return it != m_Storage.end() ? it->second : 0;
    }

}