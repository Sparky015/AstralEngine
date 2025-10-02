/**
* @file ClockTest.cpp
* @author Andrew Fagan
* @date 10/2/25
*/

#include "gtest/gtest.h"

#include "Core/Time/Clock.h"

#include <thread>

namespace Astral {

    class ClockTests : public ::testing::Test
    {
    public:

    };

    TEST_F(ClockTests, GetTime_CorrectlyReturnsTheCorrespondingUnitOfTime)
    {
        Clock testClock = Clock(); // Starts the time on construction

        std::chrono::nanoseconds timeInNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(0.2));
        std::this_thread::sleep_for(timeInNanoSeconds);

        double time = testClock.GetTimeSeconds();
        EXPECT_GT(testClock.GetTimeMilliseconds(), time);

        time = testClock.GetTimeMilliseconds();
        EXPECT_GT(testClock.GetTimeMicroseconds(), time);

        time = testClock.GetTimeMicroseconds();
        EXPECT_GT(testClock.GetTimeNanoseconds(), time);
    }

    TEST_F(ClockTests, Reset_CorrectlyResetsTheTime)
    {
        Clock testClock = Clock(); // Starts the time on construction

        std::chrono::nanoseconds timeInNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(0.3));
        std::this_thread::sleep_for(timeInNanoSeconds);

        double time = testClock.GetTimeSeconds();
        testClock.Reset();
        EXPECT_GT(time, testClock.GetTimeSeconds());
    }

}