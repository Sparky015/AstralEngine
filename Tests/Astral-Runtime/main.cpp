/**
* @file main.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "Core/Engine.h"
#include "Debug/MemoryTracking/MemoryTracker.h"
#include "Debug/Utilities/Loggers.h"
#include "gtest/gtest.h"


class StubApplication : public Application::ApplicationModule {};
IMPLEMENT_APPLICATION_MODULE(StubApplication)

class AstralEngineEnvironment : public ::testing::Environment
{
public:

    void SetUp() override
    {
        Astral::MemoryTracker::Get().Init();
        m_Engine = new Astral::Engine();
    }

    void TearDown() override
    {
        delete m_Engine;
        m_Engine = nullptr;
    }

private:
    Astral::Engine* m_Engine = nullptr;
};


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::AddGlobalTestEnvironment(new AstralEngineEnvironment);

    return RUN_ALL_TESTS();
}
