/**
* @file NoTrackingMalloc.h
* @author Andrew Fagan
* @date 2/22/25
*/

#pragma once

#ifdef ASTRAL_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace Astral {

    inline void* RawMalloc(size_t size)
    {
        #ifdef ASTRAL_PLATFORM_MACOS
            static auto rawMalloc = (void*(*)(size_t))dlsym(RTLD_NEXT, "malloc");
        #endif

        return rawMalloc(size);
    }

    inline void RawFree(void* ptr)
    {
        #ifdef ASTRAL_PLATFORM_MACOS
            static auto rawFree = (void(*)(void*))dlsym(RTLD_NEXT, "free");
        #endif
        rawFree(ptr);
    }


}

#define ASTRAL_NO_TRACKING_MALLOC(size) Astral::RawMalloc(size)
#define ASTRAL_NO_TRACKING_FREE(pointer) Astral::RawFree(pointer)