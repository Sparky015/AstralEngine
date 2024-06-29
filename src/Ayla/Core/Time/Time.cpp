//
// Created by Andrew Fagan on 6/25/24.
//

#include "Time.h"

namespace Ayla::Core::Time{

    Clock* Clock::m_instance = nullptr;

    Clock& Clock::get() {
        if (m_instance == nullptr){
            AY_TRACE("Clock: Making 'new Clock()'");
            m_instance = new Clock();
        }
        return *m_instance;
    }

    Clock::Clock():
    m_deltaTime(0),
    m_previousTimeStamp(std::chrono::high_resolution_clock::now()),
    m_clockStartingTimeStamp(std::chrono::high_resolution_clock::now()),
    m_stopWatchTime(0)
    {

    }

    Clock::~Clock(){
        delete m_instance;
        m_instance = nullptr;
    }

    void Clock::updateDeltaTime() {
        TimeStamp currentTimeStamp = std::chrono::high_resolution_clock::now();
        m_deltaTime = currentTimeStamp - m_previousTimeStamp;
        m_previousTimeStamp = currentTimeStamp;
    }

    long long Clock::getDeltaTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_deltaTime).count();
    }

    long long Clock::getStopwatchTime() {
        TimeStamp currentTimeStamp = std::chrono::high_resolution_clock::now();
        m_stopWatchTime = currentTimeStamp - m_clockStartingTimeStamp;
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_stopWatchTime).count();
    }

    void Clock::resetStopwatch() {
        m_clockStartingTimeStamp = std::chrono::high_resolution_clock::now();
    }
}