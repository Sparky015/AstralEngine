//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once

#include <sstream>

// We ignore the TURN_OFF_DEBUG_MACROS macro for the AE_ERROR call as if a branch calls AE_ERROR, it is considered a
// fatal error and should be called regardless of the build type.

/** Throws an error with a message outputted to the console. */
#define AE_ERROR(errorMessage) { std::ostringstream ss; ss << errorMessage; Astral::Macros::macro_ERROR(ss, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}


namespace Astral::Macros {

/** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_ERROR(std::ostringstream& errorMessage, const char* fileName, const int lineNumber, const char* func);

} // namespace Debug::Macros
