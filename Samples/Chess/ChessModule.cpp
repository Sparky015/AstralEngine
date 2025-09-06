/**
* @file ChessModule.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "ChessModule.h"

ChessModule* ChessModule::m_Instance = nullptr;

ChessModule::ChessModule()
{
    ASSERT(m_Instance == nullptr, "ChessModule was already constructed! Only construct one ChessModule");
    ChessModule::m_Instance = this;
}

void ChessModule::Init()
{
    PROFILE_SCOPE("ChessModule::Init")
    TRACE("Initializing Chess Module")
    m_BoardManager.Init();
    m_GameManager.Init();
    m_DebugManager.Init();

    Astral::Engine::Get().GetImGuiManager().LoadImGuiConfigFile(std::string(CHESS_SOURCE_DIR) + "imgui-config.ini");
    Astral::Engine::Get().GetWindowManager().GetWindow().SetWindowName("Chess");
    Astral::Engine::Get().GetWindowManager().GetWindow().SetWindowDimensions(800, 800);
}

void ChessModule::Update(const Astral::DeltaTime& deltaTime)
{
    if (Astral::InputState::IsKeyDown(Keycode::KEY_F))
    {
        LOG("Delta Time: " << deltaTime.GetSeconds());
    }
}

void ChessModule::Shutdown()
{
    PROFILE_SCOPE("ChessModuleShutdown")
    TRACE("Shutting down Application")
    m_DebugManager.Shutdown();
    m_BoardManager.Shutdown();
    m_GameManager.Shutdown();
}


IMPLEMENT_APPLICATION_MODULE(ChessModule);


