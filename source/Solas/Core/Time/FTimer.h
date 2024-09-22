//
// Created by Andrew Fagan on 7/5/24.
//

#pragma once

#include "TimeLiterals.h"


namespace Solas::Core::Time
{
    /** Contains the details needed to maintain a timer. */
    struct FTimer
    {
    public:
        /** Constructor */
        explicit FTimer(float amountOfTime, std::function<void()> callback);

        /** Destructor */
        ~FTimer() = default;

        /** The time stamp of when the timer was created. */
        TimeStamp StartingTimeStamp;

        /** The amount of time that the timer lasts before calling the callback. */
        float LengthOfTimer;

        /** The function callback for when the timer ends. */
        std::function<void()> Callback;

        /** Needs a comparison operator to be able to remove FTimers from a vector */
        bool operator==(const FTimer &other) const noexcept;

        [[nodiscard]] int GetTimerId() const;

    private:
        int m_TimerID;
        static int m_TimerIDTracker;
    };


}
