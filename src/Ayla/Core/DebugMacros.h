//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once


#include "Ayla/aypch.h"

// TODO: Add the macros here and rename core to something else that is more fitting

/** Macro Land */
namespace
{
    /** Outputs the message to the console with a time stamp. */
    void macro_AY_TRACE(const std::string&& title);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message to console. */
    void macro_AY_ASSERT(const bool expression, const std::string&& errorMessage);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    void macro_AY_ASSERT_SS(const bool expression, const std::ostream& errorMessage);

    /** A simple log to console that standardizes which side of the string the new line character is on. */
    void macro_AY_LOG(const std::string&& message);

    /** Throws an error with a message outputted to the console. */
    void macro_AY_ERROR(const std::string&& message);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_AY_ERROR_SS(bool expression, const std::ostream& errorMessage);

    /** Profiles a scope and outputs the time to the console. */
    class AY_PROFILER {
    public:
        explicit AY_PROFILER(std::string&& title = "Profiler");
        ~AY_PROFILER();
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;
        std::chrono::time_point<std::chrono::steady_clock> m_endTime;
        std::string m_title;
    };

}

/** Outputs the message to the console with a time stamp. */
#define AY_TRACE(title) macro_AY_TRACE(title)

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message to console. */
#define AY_ASSERT(expression, errorMessage) macro_AY_ASSERT(expression, errorMessage)

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
#define AY_ASSERT_SS(expression, errorMessage) { std::stringstream ss; macro_AY_ASSERT_SS(expression, ss << errorMessage); } // Stringstream alternative assert, so you can use an ostream for the debug message

/** A simple log to console that standardizes which side of the string the new line character is on. */
#define AY_LOG(message) macro_AY_LOG(message)

/** Profiles a scope and outputs the time to the console. */
#define AY_PROFILE_SCOPE(title) AY_PROFILER(title)

/** Throws an error with a message outputted to the console. */
#define AY_ERROR(errorMessage) macro_AY_ERROR(errorMessage)

/** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
#define AY_ERROR_SS(errorMessage) { std::stringstream ss; macro_AY_ERROR_SS(ss << errorMessage); } // Stringstream alternative assert, so you can use an ostream for the debug message

