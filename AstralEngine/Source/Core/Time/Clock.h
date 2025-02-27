/**
* @file Clock.h
* @author Andrew Fagan
* @date 1/12/2025
*/

#pragma once

#include <chrono>

namespace Core {

    /**@class Clock
     * @brief Provides a stopwatch for a way to measure how much time has passed between things or events. */
    class Clock
    {
    public:
        Clock();
        ~Clock() = default;

        /**@brief Returns the current stopwatch time in seconds. */
        [[nodiscard]] float GetTimeSeconds() const;

        /**@brief Returns the current stopwatch time in milliseconds. */
        [[nodiscard]] size_t GetTimeMilliseconds() const;

        /**@brief Returns the current stopwatch time in microseconds. */
        [[nodiscard]] size_t GetTimeMicroseconds() const;

        /**@brief Returns the current stopwatch time in nanoseconds. */
        [[nodiscard]] size_t GetTimeNanoseconds() const;

        /**@brief Resets the stopwatch. */
        void Reset();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_InitialTimePoint;
    };

}