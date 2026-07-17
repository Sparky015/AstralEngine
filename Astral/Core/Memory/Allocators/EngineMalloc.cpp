/**
* @file EngineMalloc.cpp
* @author Andrew Fagan
* @date 7/16/26
*/

#include "EngineMalloc.h"

namespace Astral {

    EngineMalloc::EngineMalloc() :
        m_TLSFAllocator(TLSFAllocator(4'000'000))
    {
    }


    EngineMalloc& EngineMalloc::Get()
    {
        static EngineMalloc engineMallocInstance = EngineMalloc();
        return engineMallocInstance;
    }


    void* EngineMalloc::Allocate(size_t size)
    {
        std::unique_lock lock(m_AllocatorMutex);
        return m_TLSFAllocator.Allocate(size);
    }


    void EngineMalloc::Free(void* pointer)
    {
        std::unique_lock lock(m_AllocatorMutex);
        m_TLSFAllocator.Free(pointer);
    }

}
