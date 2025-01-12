/**
* @file DeltaTime.h
* @author Andrew Fagan
* @date 1/12/2025
*/

#pragma once

#include <chrono>

namespace Core {

    /**@class DeltaTime
     * @brief Stores the delta time of each frame. Needs to be updated every frame. */
    class DeltaTime
    {
    public:
        DeltaTime();

        /**@brief Calculates the delta time value.
         * @note Should be called every frame to get accurate delta times. */
        void UpdateDeltaTime();

        /**@brief Returns the delta time in seconds. */
        [[nodiscard]] float GetSeconds() const;

        /**@brief Returns the delta time in milliseconds. */
        [[nodiscard]] float GetMilliseconds() const;

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTimePoint;
        float m_DeltaTime;
    };

}