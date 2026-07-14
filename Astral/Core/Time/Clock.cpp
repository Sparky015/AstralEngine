/**
* @file Clock.cpp
* @author Andrew Fagan
* @date 1/12/2025
*/

#include "Clock.h"

namespace Astral {

    Clock::Clock() : m_InitialTimePoint(std::chrono::high_resolution_clock::now()) {}

    float Clock::GetTimeSeconds() const
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTimePoint - m_InitialTimePoint).count() / 1000.0f;
    }

    size_t Clock::GetTimeMilliseconds() const
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTimePoint - m_InitialTimePoint).count();
    }

    size_t Clock::GetTimeMicroseconds() const
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(currentTimePoint - m_InitialTimePoint).count();
    }

    size_t Clock::GetTimeNanoseconds() const
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTimePoint - m_InitialTimePoint).count();
    }

    void Clock::Reset()
    {
        m_InitialTimePoint = std::chrono::high_resolution_clock::now();
    }

}