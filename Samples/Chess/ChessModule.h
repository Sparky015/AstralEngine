/**
* @file ChessModule.h
* @author Andrew Fagan
* @date 4/19/25
*/

#pragma once

#include "Astral.h"


#include "Chessboard/ChessboardManager.h"
#include "Core/Events/EventListener.h"
#include "Debug/DebugManager.h"
#include "GameManager.h"
#include "Core/Engine.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Input/InputState.h"
#include "Window/WindowManager.h"


class ChessModule : public ApplicationModule
{
public:
    ChessModule();
    ~ChessModule() override = default;

    void Init() override;
    void Update(const Astral::DeltaTime& deltaTime) override;
    void Shutdown() override;

    static ChessModule& Get() { return *ChessModule::m_Instance; }

    Game::ChessboardManager& GetBoardManager() { return m_BoardManager; }

private:
    static ChessModule* m_Instance;

    Astral::ChessDebugManager m_DebugManager;
    Game::ChessboardManager m_BoardManager;
    Game::GameManager m_GameManager;
};