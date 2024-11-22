//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once

#include <cassert>
#include <chrono>
#include <string>
#include <sstream>


/** Outputs the message to the console with a time stamp. */
#define TRACE(message) { std::ostringstream ss; ss << message; Debug::Macros::macro_TRACE(ss); }

/** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
#define LOG(message) { std::ostringstream ss; ss << message; Debug::Macros::macro_LOG(ss); }

/** Logs warnings (that can be a stream) to the console with a yellow color. */
#define WARN(message) { std::ostringstream ss;  ss << message; Debug::Macros::macro_WARN(ss, __FILE_NAME__, __LINE__); }

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
#define ASSERT(expression, errorMessage) { std::ostringstream ss; ss << errorMessage; assert(Debug::Macros::macro_ASSERT(expression, ss)); }

/** Throws an error with a message outputted to the console. */
#define ERROR(errorMessage) { std::ostringstream ss; ss << errorMessage; assert(Debug::Macros::macro_ERROR(ss)); }

/** Profiles a scope and outputs the time to the console. */
#define PROFILE_SCOPE() Debug::Macros::macro_SCOPE_PROFILER localScopedProfiler = Debug::Macros::macro_SCOPE_PROFILER(__PRETTY_FUNCTION__);


/** Macro Land */
namespace Debug::Macros {
    
    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_TRACE(const std::ostringstream& message);

    /** A simple log to console that can take a stream while also standardizing which side of the string the new line character is on. */
    void macro_LOG(const std::ostringstream& message);

    /** Logs warnings (that can be a stream) to the console with a yellow color. */
    void macro_WARN(const std::ostringstream& message, const char* file, int line);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    bool macro_ASSERT(const bool expression, std::ostringstream& errorMessage);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    bool macro_ERROR(std::ostringstream& errorMessage);

    /** Profiles a scope and outputs the time to the console. */
    class macro_SCOPE_PROFILER {
    public:
        explicit macro_SCOPE_PROFILER(std::string&& title);
        ~macro_SCOPE_PROFILER();
    private:
        std::string m_title;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;


    };

    /** Checks if std::cout is in fail state and corrects it. */
    void CheckIfCoutFailed();

} // namespace Debug::Macros

