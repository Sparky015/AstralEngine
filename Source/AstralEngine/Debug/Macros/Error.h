//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once


#ifndef TURN_OFF_DEBUG_MACROS

/** Throws an error with a message outputted to the console. */
#define ERROR(errorMessage) { std::ostringstream ss; ss << errorMessage; Debug::Macros::macro_ERROR(ss, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}

#else
#define ERROR(errorMessage)
#endif


namespace Debug::Macros {

/** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_ERROR(std::ostringstream& errorMessage, const char* fileName, const int lineNumber, const char* func);

} // namespace Debug::Macros
