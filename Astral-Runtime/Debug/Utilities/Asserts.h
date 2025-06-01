//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once

#include <sstream>

#ifndef TURN_OFF_DEBUG_MACROS

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message to console. */
//#define ASSERT(expression, errorMessage) [[unlikely]] if (!(expression)) {Debug::Macros::macro_ASSERT(#expression, errorMessage, __FILE__, __LINE__, __func__); throw std::runtime_error(errorMessage);}
#define ASSERT(expression, errorMessage) [[unlikely]] if (!(expression)) {std::ostringstream ss; ss << errorMessage; Astral::Macros::macro_ASSERT(#expression, ss, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}

#else
#define ASSERT(expression, errorMessage)
#endif


namespace Astral::Macros {

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    void macro_ASSERT(const char* expressionString, const char* errorMessage, const char* file, const int lineNumber, const char* func);
    void macro_ASSERT(const char* expressionString, std::ostringstream& errorMessage, const char* file, const int lineNumber, const char* func);

}