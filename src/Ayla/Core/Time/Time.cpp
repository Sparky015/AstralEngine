//
// Created by Andrew Fagan on 6/25/24.
//

#include "Time.h"

namespace Ayla::Core::Time{

    Clock* Clock::m_Instance = nullptr;

    Clock& Clock::Get() {
        if (m_Instance == nullptr){
            AY_TRACE("[Sholas] Clock: Initializing Clock");
            m_Instance = new Clock();
        }
        return *m_Instance;
    }

    Clock::Clock() :
            m_DeltaTime(0),
            m_PreviousTimeStamp(std::chrono::high_resolution_clock::now()),
            m_ClockStartingTimeStamp(std::chrono::high_resolution_clock::now()),
            m_StopWatchTime(0)
    {}


    Clock::~Clock()
    {
        delete m_Instance;
        m_Instance = nullptr;
    }


    void Clock::UpdateDeltaTime()
    {
        TimeStamp currentTimeStamp = std::chrono::high_resolution_clock::now();
        m_DeltaTime = currentTimeStamp - m_PreviousTimeStamp;
        m_PreviousTimeStamp = currentTimeStamp;
    }


    /** Returns the time in milliseconds. */
    long long Clock::GetDeltaTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_DeltaTime).count();
    }


    /** Returns the time in milliseconds. */
    long long Clock::GetStopwatchTime()
    {
        TimeStamp currentTimeStamp = std::chrono::high_resolution_clock::now();
        m_StopWatchTime = currentTimeStamp - m_ClockStartingTimeStamp;
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_StopWatchTime).count();
    }


    void Clock::ResetStopwatch()
    {
        m_ClockStartingTimeStamp = std::chrono::high_resolution_clock::now();
    }

}