//
// Created by Andrew Fagan on 6/25/24.
//

#include "Time.h"

namespace Ayla::Core::Time
{
    Clock* Clock::m_Instance = nullptr;

    void Clock::Init()
    {
        AY_ASSERT(m_Instance == nullptr, "[Ayla] Core/Time.cpp: Clock has already been initialized!");
        AY_TRACE("[Sholas] Clock: Initializing Clock");
        m_Instance = new Clock();
    }


    Clock& Clock::Get()
    {
        AY_ASSERT(m_Instance != nullptr, "[Ayla] Core/Time.cpp: Clock has not been initialized!");
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
    long long Clock::GetDeltaTime() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_DeltaTime).count();
    }


    /** Returns the time in milliseconds. */
    long long Clock::GetStopwatchTime() const
    {
        TimeStamp currentTimeStamp = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeStamp - m_ClockStartingTimeStamp).count();
    }


    void Clock::ResetStopwatch()
    {
        m_ClockStartingTimeStamp = std::chrono::high_resolution_clock::now();
    }

}