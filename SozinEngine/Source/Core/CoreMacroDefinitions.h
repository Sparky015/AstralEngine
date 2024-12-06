//
// Created by Andrew Fagan on 10/12/24.
//

#pragma once

/** Removing the _t from specific size types for ease of use. */
#include <cstdint>

#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define int64 int64_t

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t


//#define PLATFORM_MACOS


/** Defines for each build config */

/** Debug build with logging and symbols. */
#ifdef DEBUG_BUILD

#endif


/** Release build that can be used for profiling performance issues. */
#ifdef RELEASE_WITH_DEBUG_INFO_BUILD
#define TURN_OFF_DEBUG_MACROS
#endif


/** Final release build with compiler optimizations. */
#ifdef RELEASE_BUILD
#define TURN_OFF_DEBUG_MENU
#define TURN_OFF_DEBUG_MACROS
#define TURN_OFF_PROFILER_MACRO
#define TURN_OFF_LOGGING_CONSOLE_TO_FILE
#endif


/** Release build with a smaller executable size with some compiler optimizations. */
#ifdef MINIMUM_SIZE_RELEASE_BUILD
#define TURN_OFF_DEBUG_MENU
#define TURN_OFF_DEBUG_MACROS
#define TURN_OFF_PROFILER_MACRO
#define TURN_OFF_LOGGING_CONSOLE_TO_FILE
#endif