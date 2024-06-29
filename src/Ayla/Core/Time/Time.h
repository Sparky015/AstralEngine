//
// Created by Andrew Fagan on 6/25/24.
//

#pragma once

namespace Ayla::Core::Time {

    class Clock {
    public:
        using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
        using TimeDuration = std::chrono::duration<float>;

        static Clock& get();
        void updateDeltaTime();
        long long getDeltaTime();
        long long getStopwatchTime();
        void resetStopwatch();

        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
        Clock(Clock&&) = delete;
        Clock& operator=(Clock&&) = delete;

    private:


        static Clock* m_instance;
        TimeDuration m_deltaTime;
        TimeStamp m_previousTimeStamp;

        TimeDuration m_stopWatchTime{};
        TimeStamp m_clockStartingTimeStamp;

        Clock();
        ~Clock();

    };

}
