//
// Created by Andrew Fagan on 4/14/24.
//

#pragma once


/** Defines the ALYA_API for compiling Ayla into a dll/shared library file */
#ifdef AYLA_PLATFORM_WINDOWS
    #ifdef AYLA_BUILD_SHARED_LIB
        #define AYLA_API __declspec(dllexport)        // Export
    #else
        #define AYLA_API __declspec(dllimport)        // Import
    #endif
#elif AYLA_PLATFORM_MACOS
    #ifdef AYLA_BUILD_SHARED_LIB
        #define AYLA_API __attribute__((visibility("default")))       // Export
    #else
        #define AYLA_API        // Import
    #endif
#else
    #error Core/CoreDefinitions.h: Unsupported/Unknown Platform!
#endif


/** Defines the unknown platform macro if no supported platforms are defined */
#ifndef AYLA_PLATFORM_MACOS
    #ifndef AYLA_PLATFORM_WINDOWS
        #define AYLA_PLATFORM_UNKNOWN
    #endif
#endif
