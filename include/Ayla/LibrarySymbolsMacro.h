//
// Created by Andrew Fagan on 4/14/24.
//

#pragma once

#ifdef AYLA_PLATFORM_WINDOWS
    #ifdef AYLA_BUILD_SHARED
        #define AYLA_API __declspec(dllexport)        // Export
    #else
        #define AYLA_API __declspec(dllimport)        // Import
    #endif
#elif AYLA_PLATFORM_APPLE
    #ifdef AYLA_BUILD_SHARED
        #define AYLA_API __attribute__((visibility("default")))       // Export
    #else
        #define AYLA_API        // Import
    #endif
#else
    #error Something went wrong with defining Ayla API symbols (Platform not found)
#endif

