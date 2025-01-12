/**
* @file Clock.cpp
* @author Andrew Fagan
* @date 1/12/2025
*/

#include "Clock.h"

namespace Core {

    Clock::Clock() : m_InitialTimePoint(std::chrono::high_resolution_clock::now()) {}

    float Clock::GetTime() const
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTimePoint - m_InitialTimePoint).count() / 1000.0f;
    }

    void Clock::Reset()
    {
        m_InitialTimePoint = std::chrono::high_resolution_clock::now();
    }

}