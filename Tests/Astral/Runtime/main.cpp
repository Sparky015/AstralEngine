/**
* @file main.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Core/Utilities/Loggers.h"
#include "gtest/gtest.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"


class StubApplication : public Application::ApplicationModule {};
IMPLEMENT_APPLICATION_MODULE(StubApplication)

class AstralEngineEnvironment : public ::testing::Environment
{
public:

    void SetUp() override
    {
        m_Engine = new Astral::Engine();
        m_Engine->GetWindowManager().Init();
        m_Engine->GetRendererManager().InitOnlyRenderingContext();
        m_Engine->GetAssetManager().Init();
    }

    void TearDown() override
    {
        m_Engine->GetAssetManager().Shutdown();
        m_Engine->GetRendererManager().Shutdown();
        m_Engine->GetWindowManager().Shutdown();
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
