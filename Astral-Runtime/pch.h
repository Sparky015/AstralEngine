/**
* @file pch.h
* @author Andrew Fagan
* @date 10/12/2024
* @brief Precompiled Header File
*/

#pragma once


#include <iostream>             // C++ IO streams

#include <array>                // std::array
#include <cstring>              // C-style string operations
#include <memory>               // Smart pointers
#include <string>               // std::string
#include <vector>               // std::vector

#include <algorithm>            // STL algorithms
#include <thread>               // Threading support
#include <utility>              // Pair, move, forward



// Other Headers
#include "Core/CoreMacroDefinitions.h"
#include "Core/Math/Math.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Loggers.h"
