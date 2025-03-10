/**
* @file DeltaTime.cpp
* @author Andrew Fagan
* @date 1/12/2025
*/
#include "DeltaTime.h"


namespace Core {

    DeltaTime::DeltaTime() : m_DeltaTime(0) {}

    void DeltaTime::UpdateDeltaTime()
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> currentTimePoint = std::chrono::high_resolution_clock::now();
        m_DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePoint - m_LastTimePoint).count();
        m_LastTimePoint = currentTimePoint;
    }

    float DeltaTime::GetSeconds() const
    {
        return m_DeltaTime / 1000000.0f;
    }

    float DeltaTime::GetMilliseconds() const
    {
        return m_DeltaTime / 1000.0f;
    }

    float DeltaTime::GetMicroseconds() const
    {
        return m_DeltaTime;
    }

}