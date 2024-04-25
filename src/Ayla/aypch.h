//
// Created by Andrew Fagan on 4/18/24.
//
/**
 * @file
 * @brief This file is for precompiled headers.
 */

#pragma once

// General Stuffs
#include <iostream> ///< Standard Input/Output stream library
#include <algorithm> ///< Standard algorithm library
#include <functional> ///< Standard functional library
#include <memory> ///< Standard memory management library
#include <thread> ///< Standard threading library
#include <utility> ///< Standard utility library
#include <cstdio> ///< Standard I/O library

// Data Structures
#include <string> ///< Standard string library
#include <stack> ///< Standard stack data structure
#include <deque> ///< Standard dequeue data structure
#include <queue> ///< Standard queue data structure
#include <array> ///< Standard array data structure
#include <vector> ///< Standard vector data structure
#include <set> ///< Standard set data structure
#include <map> ///< Standard map data structure
#include <unordered_map> ///< Standard unordered map data structure
#include <unordered_set> ///< Standard unordered set data structure

// Other Stuff
#include "Ayla/Core/Core.h" ///< Core functionality of Ayla project

/// Check if the platform is Windows
#ifdef AYLA_PLATFORM_WINDOWS
#include <Windows.h> ///< Windows specific includes
#endif