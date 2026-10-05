/**
* @file DeltaTimeTest.cpp
* @author Andrew Fagan
* @date 10/2/25
*/

/**
* @file ClockTest.cpp
* @author Andrew Fagan
* @date 10/2/25
*/

#include "gtest/gtest.h"

#include "Core/Time/DeltaTime.h"

#include <thread>

namespace Astral {

    class DeltaTimeTests : public ::testing::Test
    {
    public:

    };

    TEST_F(DeltaTimeTests, GetTime_CorrectlyReturnsTheCorrespondingUnitOfTime)
    {
        DeltaTime testDeltaTime = DeltaTime(); // Starts the time on construction

        using namespace std::chrono;

        nanoseconds timeInNanoSeconds = duration_cast<nanoseconds>(duration<double>(0.2));
        std::this_thread::sleep_for(timeInNanoSeconds);

        double time = testDeltaTime.GetSeconds();
        EXPECT_GT(testDeltaTime.GetMilliseconds(), time);

        time = testDeltaTime.GetMilliseconds();
        EXPECT_GT(testDeltaTime.GetMicroseconds(), time);
    }

}
