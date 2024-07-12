//
// Created by Andrew Fagan on 6/25/24.
//

#pragma once

#include "TimeLiterals.h"
#include "FTimer.h"

namespace Ayla::Core::Time
{
    /**
     * Provides delta time as well as a stopwatch for a way to measure how much time has passed between things or events.
     * @note Clock is a singleton, so there is only one clock that is engine-wide.
     */
    class Clock final
    {
    public:

        /** Gives access to the Clock singleton. */
        static void Init();

        /** Gives access to the Clock singleton. */
        static Clock& Get();

        /** Retrieves the current time stamp */
        [[nodiscard]] TimeStamp GetCurrentTimeStamp() const;

        /**
         * Calculates the delta time value.
         * @note Needs to be called every frame.
         */
        void UpdateDeltaTime();

        /**
         * Checks the timers to see if any have elapsed.
         * @note May need to be called more than once a frame to keep the timers precise
         */
        void CheckTimers();

        /** Returns the delta time. */
        [[nodiscard]] long long GetDeltaTime() const;

        /** Returns the current stopwatch time. */
        [[nodiscard]] long long GetStopwatchTime() const;

        /** Resets the stopwatch. */
        void ResetStopwatch();

        /** Adds a timer that has a callback for tracking time across function calls */
        void AddTimer(float amountOfTime, const std::function<void()>& callback);

        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
        Clock(Clock&&) = delete;
        Clock& operator=(Clock&&) = delete;

    private:
        Clock();
        ~Clock();
        static Clock* m_Instance;
        TimeDuration m_DeltaTime;
        TimeStamp m_PreviousTimeStamp;
        TimeStamp m_ClockStartingTimeStamp;
        std::vector<FTimer> m_Timers;
    };

}
