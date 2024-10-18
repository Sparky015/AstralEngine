//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once

#include <cassert>
#include <chrono>
#include <string>
#include <sstream>


/** Outputs the message to the console with a time stamp. */
#define TRACE(message) { std::stringstream ss; Debug::Macros::macro_TRACE(ss << message); }

/** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
#define LOG(message) { std::stringstream ss; Debug::Macros::macro_LOG(ss << message); }

/** Logs warnings (that can be a stream) to the console with a yellow color. */
#define WARN(message) { std::stringstream ss; Debug::Macros::macro_WARN(ss << message, __FILE_NAME__, __LINE__); }

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
#define ASSERT(expression, errorMessage) { std::stringstream ss; assert(Debug::Macros::macro_ASSERT(expression, ss << errorMessage)); }

/** Throws an error with a message outputted to the console. */
#define ERROR(errorMessage) { std::stringstream ss; assert(Debug::Macros::macro_ERROR(ss << errorMessage)); }

/** Profiles a scope and outputs the time to the console. */
#define PROFILE_SCOPE(title) Solas::Debug::Macros::PROFILER localScopedProfiler = Debug::Macros::PROFILER(title);


/** Macro Land */
namespace Debug::Macros {

    /** Initializes the file stream for logging the macro outputs. This includes the creation of the folder structure that contains the logs. */
    void InitLogFileForMacros();

    /** Closes the file stream for logging macro outputs. */
    void CloseLogFileForMacros();

    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_TRACE(const std::ostream& message);

    /** A simple log to console that can take a stream while also standardizing which side of the string the new line character is on. */
    void macro_LOG(const std::ostream& message);

    /** Logs warnings (that can be a stream) to the console with a yellow color. */
    void macro_WARN(const std::ostream& message, const char* file, int line);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    bool macro_ASSERT(const bool expression, const std::ostream& errorMessage);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    bool macro_ERROR(const std::ostream& errorMessage);

    /** Profiles a scope and outputs the time to the console. */
    class AY_PROFILER {
    public:
        explicit AY_PROFILER(std::string&& title = "Profiler");
        ~AY_PROFILER();
    private:
        std::string m_title;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    };

    /** Checks if std::cout is in fail state and corrects it. */
    void CheckIfCoutFailed();

} // namespace Debug::Macros

