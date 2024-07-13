//
// Created by Andrew Fagan on 7/12/24.
//

#pragma once

#include "Ayla/aypch.h"


/** Outputs the message to the console with a time stamp. */
#define AY_TRACE(title) Ayla::Core::Macros::macro_AY_TRACE(title)

/** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
#define AY_ASSERT(expression, errorMessage) { std::stringstream ss; Ayla::Core::Macros::macro_AY_ASSERT(expression, ss << errorMessage); }

/** A simple log to console that standardizes which side of the string the new line character is on. */
#define AY_LOG(message) Ayla::Core::Macros::macro_AY_LOG(message)

/** Throws an error with a message outputted to the console. */
#define AY_ERROR(errorMessage) { std::stringstream ss; Ayla::Core::Macros::macro_AY_ERROR(ss << errorMessage); }

/** Profiles a scope and outputs the time to the console. */
#define AY_PROFILE_SCOPE(title) Ayla::Core::Macros::AY_PROFILER(title)


/** Macro Land */
namespace Ayla::Core::Macros {

    /** Outputs the message to the console with a time stamp [hr:min:sec:ms]. */
    void macro_AY_TRACE(const std::string&& title);

    /** Asserts a conditional. Throws an error if the conditional isn't true and outputs a message (that can come in the form of an ostream) to console. */
    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage);

    /** A simple log to console that standardizes which side of the string the new line character is on. */
    void macro_AY_LOG(const std::string&& message);

    /** Throws an error with a message (that can come in the form of an ostream) outputted to the console. */
    void macro_AY_ERROR(const std::ostream& errorMessage);

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

} // namespace Ayla::Core::Macros

