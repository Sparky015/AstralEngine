/**
* @file PointerAllocationSizeMap.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "Tracking/PointerAllocationSizeMap.h"

namespace Core {


    void PointerAllocationSizeMap::AddPointer(void* pointer, size_t size)
    {
        m_Storage[pointer] = size;
    }


    void PointerAllocationSizeMap::FreePointer(void* pointer)
    {
        m_Storage[pointer] = 0;
    }


    size_t PointerAllocationSizeMap::GetPointerSize(void* pointer)
    {
        return m_Storage[pointer];
    }

}