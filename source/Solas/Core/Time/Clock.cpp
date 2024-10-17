//
// Created by Andrew Fagan on 6/25/24.
//

#include "Clock.h"

namespace Solas::Core::Time
{
    Clock* Clock::m_Instance = nullptr;

    void Clock::Init()
    {
        AY_ASSERT(m_Instance == nullptr, "[Solas] Core/Time.cpp: Clock has already been initialized!");
        AY_TRACE("[Solas] Clock: Initializing Clock");
        m_Instance = new Clock();
    }


    Clock& Clock::Get()
    {
        AY_ASSERT(m_Instance != nullptr, "[Solas] Core/Time.cpp: Clock has not been initialized!");
        return *m_Instance;
    }


    void Clock::Destroy()
    {
        delete m_Instance;
        m_Instance = nullptr;
    }


    Clock::Clock() :
        m_DeltaTime(0),
        m_PreviousTimeStamp(GetCurrentTimeStamp()),
        m_ClockStartingTimeStamp(GetCurrentTimeStamp())
    {}


    Clock::~Clock()
    {
        AY_TRACE("[Solas] Clock: Destroying Clock");
    }


    TimeStamp Clock::GetCurrentTimeStamp() const
    {
        return std::chrono::high_resolution_clock::now();
    }


    void Clock::UpdateDeltaTime()
    {
        const TimeStamp currentTimeStamp = GetCurrentTimeStamp();
        m_DeltaTime = currentTimeStamp - m_PreviousTimeStamp;
        m_PreviousTimeStamp = currentTimeStamp;
    }


    void Clock::CheckTimers() {
        const TimeStamp currentTimeStamp = GetCurrentTimeStamp();
        float elapsedTime = 0;
        for (const FTimer& timer : m_Timers)
        {
            elapsedTime = (float)std::chrono::duration_cast<std::chrono::seconds>(currentTimeStamp - timer.StartingTimeStamp).count();
            if (elapsedTime >= timer.LengthOfTimer)
            {
                try
                {
                    timer.Callback();
                    m_Timers.erase(std::remove(m_Timers.begin(), m_Timers.end(), timer), m_Timers.end());
                }
                catch(std::exception& exception)
                {
                    AY_ERROR("Timer callback function caused an error of : " << exception.what());
                }
            }
        }
    }


    /** Returns the time in milliseconds. */
    float Clock::GetDeltaTime() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_DeltaTime).count() * .001;
    }


    /** Returns the time in milliseconds. */
    float Clock::GetStopwatchTime() const
    {
        const TimeStamp currentTimeStamp = GetCurrentTimeStamp();
        return std::chrono::duration_cast<std::chrono::microseconds>(currentTimeStamp - m_ClockStartingTimeStamp).count() * .001;
    }


    void Clock::ResetStopwatch()
    {
        m_ClockStartingTimeStamp = GetCurrentTimeStamp();
    }


    void Clock::AddTimer(float amountOfTime, const std::function<void()>& callback) {
        m_Timers.emplace_back(amountOfTime, callback);
    }

} // namespace Solas::Core::Time