//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once

#include <chrono>
#include <string>


/** Outputs the message to the console with a time stamp. */
#define AY_TRACE(title) Ayla::Debug::Macros::macro_AY_TRACE(title)

/** A simple log to console that standardizes which side of the string the new line character is on. */
#define AY_LOG(message) Ayla::Debug::Macros::macro_AY_LOG(message)

/** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
#define AY_LOG_SS(message) { std::stringstream ss; Ayla::Debug::Macros::macro_AY_LOG_SS(ss << message); }

/** Logs warnings to the console with a yellow color. */
#define AY_WARN(message) Ayla::Debug::Macros::macro_AY_WARN(message)

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
#define AY_ASSERT(expression, errorMessage) { std::stringstream ss; Ayla::Debug::Macros::macro_AY_ASSERT(expression, ss << errorMessage); }

/** Throws an error with a message outputted to the console. */
#define AY_ERROR(errorMessage) { std::stringstream ss; Ayla::Debug::Macros::macro_AY_ERROR(ss << errorMessage); }

/** Profiles a scope and outputs the time to the console. */
#define AY_PROFILE_SCOPE(title) Ayla::Debug::Macros::AY_PROFILER localScopedProfiler = Ayla::Debug::Macros::AY_PROFILER(title);


/** Macro Land */
namespace Ayla::Debug::Macros {

    /** Initializes the file stream for logging the macro outputs. This includes the creation of the folder structure that contains the logs. */
    void initLogForMacros();

    /** Closes the file stream for logging macro outputs. */
    void closeLogForMacros();

    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_AY_TRACE(const std::string&& title);

    /** A simple log to console that standardizes which side of the string the new line character is on. */
    void macro_AY_LOG(const std::string&& message);

    /** A simple log to console that can take a stream and standardizes which side of the string the new line character is on. */
    void macro_AY_LOG_SS(const std::ostream& message);

    /** Logs warnings to the console with a yellow color. */
    void macro_AY_WARN(const std::string&& message);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_AY_ERROR(const std::ostream& errorMessage);

    /** Profiles a scope and outputs the time to the console. */
    class AY_PROFILER {
    public:
        explicit AY_PROFILER(std::string&& title = "Profiler");
        ~AY_PROFILER();
    private:
        std::string m_title;
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;
        std::chrono::time_point<std::chrono::steady_clock> m_endTime;
    };

    /** Checks if std::cout is in fail state and corrects it. */
    void CheckIfCoutFailed();

} // namespace Ayla::Debug::Macros

