//
// Created by Andrew Fagan on 6/25/24.
//

#pragma once

namespace Ayla::Core::Time {


    /**
     * Provides delta time as well as a stopwatch for a way to measure how much time has passed between things or events.
     * @note Clock is a singleton, so there is only one clock that is engine-wide.
     */
    class Clock final {
    public:

        /** Shorthands to use because chrono is so verbose */
        using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
        using TimeDuration = std::chrono::duration<float>;

        /** Gives access to the Clock singleton. */
        static Clock& Get();

        /**
         * Calculates the delta time value.
         * @note Needs to be called every frame.
         */
        void UpdateDeltaTime();

        /** Returns the delta time. */
        long long GetDeltaTime();

        /** Returns the current stopwatch time. */
        long long GetStopwatchTime();

        /** Resets the stopwatch. */
        void ResetStopwatch();


        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
        Clock(Clock&&) = delete;
        Clock& operator=(Clock&&) = delete;

    private:

        static Clock* m_Instance;
        TimeDuration m_DeltaTime;
        TimeStamp m_PreviousTimeStamp;

        TimeDuration m_StopWatchTime{}; // Default initializes?
        TimeStamp m_ClockStartingTimeStamp;

        Clock();
        ~Clock();

    };

}
