/**
* @file ChessModule.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "Astral.h"

#include "Chessboard/ChessboardManager.h"
#include "Core/Events/EventListener.h"
#include "Debug/DebugManager.h"
#include "GameManager.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Input/InputState.h"

class ChessModule : public ApplicationModule
{
public:
    ~ChessModule() override = default;

    void Init() override
    {
        PROFILE_SCOPE("ChessModuleInit")
        TRACE("Initializing Application")
        Game::g_BoardManager.Init();
        Game::g_GameManager.Init();
        Debug::g_DebugManager.Init();
        Debug::g_ImGuiManager.LoadImGuiConfigFile(std::string(CHESS_SOURCE_DIR) + "imgui-config.ini");
    }

    void Update(const Core::DeltaTime& deltaTime) override
    {
        if (InputState::IsKeyDown(Keycode::KEY_F))
        {
            LOG("Delta Time: " << deltaTime.GetSeconds());
        }
    }

    void Shutdown() override
    {
        PROFILE_SCOPE("ChessModuleShutdown")
        TRACE("Shutting down Application")
        Debug::g_DebugManager.Shutdown();
        Game::g_BoardManager.Shutdown();
        Game::g_GameManager.Shutdown();
    }
};

IMPLEMENT_APPLICATION_MODULE(ChessModule);