/**
* @file pch.h
* @author Andrew Fagan
* @date 10/12/2024
* @brief Precompiled Header File
*/

#pragma once


#include <iostream>             // C++ IO streams

#include <memory>               // Smart pointers
#include <cstring>              // C-style string operations
#include <string>               // std::string
#include <vector>               // std::vector
#include <array>                // std::array

#include <algorithm>            // STL algorithms
#include <utility>              // Pair, move, forward
#include <thread>               // Threading support



// Other Headers
#include "Core/CoreMacroDefinitions.h"
#include "Core/Math.h"

#include "Debug/Macros/Asserts.h"
#include "Debug/Macros/Loggers.h"
#include "Debug/Macros/Error.h"
#include "Debug/Macros/ScopeProfiler.h"
