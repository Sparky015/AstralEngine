//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once

#include <sstream>

#ifndef TURN_OFF_DEBUG_MACROS
/** Outputs the message to the console with a time stamp. */
#define TRACE(message) { std::ostringstream ss; ss << message; Astral::Macros::macro_TRACE(ss); }

/** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
#define LOG(message) { std::ostringstream ss; ss << message; Astral::Macros::macro_LOG(ss); }

/** Logs warnings (that can be a stream) to the console with a yellow color. */
#define WARN(message) { std::ostringstream ss;  ss << message; Astral::Macros::macro_WARN(ss); }


#else
#define TRACE(message)
#define LOG(message)
#define WARN(message)
#endif


namespace Astral::Macros {

    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_TRACE(const std::ostringstream& message);

    /** A simple log to console that can take a stream while also standardizing which side of the string the new line character is on. */
    void macro_LOG(const std::ostringstream& message);

    /** Logs warnings (that can be a stream) to the console with a yellow color. */
    void macro_WARN(const std::ostringstream& message);
    void macro_WARN(const std::ostringstream& message);

    /** Checks if std::cout is in fail state and corrects it. */
    void CheckIfCoutFailed();

}