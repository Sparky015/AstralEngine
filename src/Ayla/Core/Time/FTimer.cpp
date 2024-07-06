//
// Created by Andrew Fagan on 7/5/24.
//
#include "FTimer.h"

#include <utility>
#include "Clock.h"

namespace Ayla::Core::Time
{
    int FTimer::TimerIDTracker = 0;

    FTimer::FTimer(float amountOfTime, std::function<void()> callback) :
        StartingTimeStamp(Clock::Get().GetCurrentTimeStamp()),
        LengthOfTimer(amountOfTime),
        Callback(std::move(callback)),
        m_TimerID(++TimerIDTracker)
    {}

    bool FTimer::operator==(const FTimer& other) const noexcept{
        return m_TimerID == other.GetTimerID();
    }

    int FTimer::GetTimerID() const {
        return m_TimerID;
    }
}


