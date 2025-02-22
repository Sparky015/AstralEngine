/**
* @file NoTrackingMalloc.h
* @author Andrew Fagan
* @date 2/22/25
*/

#pragma once

#include <dlfcn.h>

namespace Astral {

    inline void* RawMalloc(size_t size)
    {
        static auto rawMalloc = (void*(*)(size_t))dlsym(RTLD_NEXT, "malloc");
        return rawMalloc(size);
    }

    inline void RawFree(void* ptr)
    {
        static auto rawFree = (void(*)(void*))dlsym(RTLD_NEXT, "free");
        rawFree(ptr);
    }


}

#define ASTRAL_NO_TRACKING_MALLOC(size) Astral::RawMalloc(size)
#define ASTRAL_NO_TRACKING_FREE(pointer) Astral::RawFree(pointer)