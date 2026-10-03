/**
* @file EngineMalloc.h
* @author Andrew Fagan
* @date 7/16/26
*/

#pragma once

#include <mutex>

#include "TLSFAllocator.h"
#include "Core/FixedIntegerTypes.h"

namespace Astral {

    class EngineMalloc
    {
    public:
        EngineMalloc();
        ~EngineMalloc() = default;

        static EngineMalloc& Get();

        void* Allocate(size_t size);

        void Free(void* pointer);

    private:

        TLSFAllocator m_TLSFAllocator;
        std::mutex m_AllocatorMutex;
    };


}
