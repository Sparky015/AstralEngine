//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once

#include <cassert>
#include <chrono>
#include <string>
#include <sstream>

#ifndef TURN_OFF_DEBUG_MACROS
/** Outputs the message to the console with a time stamp. */
#define TRACE(message) { std::ostringstream ss; ss << message; Debug::Macros::macro_TRACE(ss); }

/** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
#define LOG(message) { std::ostringstream ss; ss << message; Debug::Macros::macro_LOG(ss); }

/** Logs warnings (that can be a stream) to the console with a yellow color. */
#define WARN(message) { std::ostringstream ss;  ss << message; Debug::Macros::macro_WARN(ss, __FILE__, __LINE__); }

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message to console. */
#define ASSERT(expression, errorMessage)  if (!(expression)) {Debug::Macros::macro_ASSERT(#expression, errorMessage, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}
#define ASSERT_SS(expression, errorMessage) if (!(expression)) {std::ostringstream ss; ss << (errorMessage); Debug::Macros::macro_ASSERT(#expression ss, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}

/** Throws an error with a message outputted to the console. */
#define ERROR(errorMessage) { std::ostringstream ss; ss << errorMessage; Debug::Macros::macro_ERROR(ss, __FILE__, __LINE__, __func__); throw std::runtime_error(ss.str());}

#else
#define TRACE(message)
#define LOG(message)
#define WARN(message)
#define ASSERT(expression, errorMessage)
#define ASSERT_SS(expression, errorMessage)
#define ERROR(errorMessage)
#endif

/** Profiles a scope and outputs the time to the console. */
#ifndef TURN_OFF_PROFILER_MACRO
#define PROFILE_SCOPE() Debug::Macros::macro_SCOPE_PROFILER localScopedProfiler = Debug::Macros::macro_SCOPE_PROFILER(__PRETTY_FUNCTION__);
#else
#define PROFILE_SCOPE()
#endif

/** Macro Land */
namespace Debug::Macros {
    
    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_TRACE(const std::ostringstream& message);

    /** A simple log to console that can take a stream while also standardizing which side of the string the new line character is on. */
    void macro_LOG(const std::ostringstream& message);

    /** Logs warnings (that can be a stream) to the console with a yellow color. */
    void macro_WARN(const std::ostringstream& message, const char* file, int line);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    void macro_ASSERT(const char* expressionString, const char* errorMessage, const char* file, const int lineNumber, const char* func);
    void macro_ASSERT(const char* expressionString, std::ostringstream& errorMessage, const char* file, const int lineNumber, const char* func);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_ERROR(std::ostringstream& errorMessage, const char* fileName, const int lineNumber, const char* func);


    /** Profiles a scope and outputs the time to the console. */
    class macro_SCOPE_PROFILER {
    public:
        explicit macro_SCOPE_PROFILER(const char* title) noexcept;
        ~macro_SCOPE_PROFILER() noexcept;
    private:
        const char* m_title;
        const std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    };

    /** Checks if std::cout is in fail state and corrects it. */
    void CheckIfCoutFailed();

} // namespace Debug::Macros

