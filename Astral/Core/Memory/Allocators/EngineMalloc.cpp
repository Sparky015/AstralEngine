/**
* @file EngineMalloc.cpp
* @author Andrew Fagan
* @date 7/16/26
*/

#include "EngineMalloc.h"

namespace Astral {

    EngineMalloc::EngineMalloc() :
        m_TLSFAllocator(TLSFAllocator(50'000'000))
    {
    }


    EngineMalloc& EngineMalloc::Get()
    {
        static EngineMalloc engineMallocInstance = EngineMalloc();
        return engineMallocInstance;
    }


    void* EngineMalloc::Allocate(size_t size)
    {
        if (size > 128'000)
        {
            return malloc(size);
        }

        std::unique_lock lock(m_AllocatorMutex);
        return m_TLSFAllocator.Allocate(size);
    }


    void EngineMalloc::Free(void* pointer)
    {
        std::unique_lock lock(m_AllocatorMutex);

        if (m_TLSFAllocator.DoesOwnPointer(pointer))
        {
            m_TLSFAllocator.Free(pointer);
        }
        else
        {
            free(pointer);
        }
    }

}
