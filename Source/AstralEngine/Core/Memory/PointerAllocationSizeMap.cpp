/**
* @file PointerAllocationSizeMap.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "PointerAllocationSizeMap.h"

namespace Core {


    void PointerAllocationSizeMap::AddPointer(void* pointer, size_t size)
    {
        m_Storage[pointer] = size;
    }


    void PointerAllocationSizeMap::FreePointer(void* pointer)
    {
        const auto it = m_Storage.find(pointer);
        if (it == m_Storage.end()) { return; }
        m_Storage.erase(pointer);
    }


    size_t PointerAllocationSizeMap::GetPointerSize(void* pointer)
    {
        const auto it = m_Storage.find(pointer);
        return it != m_Storage.end() ? it->second : 0;
    }

}