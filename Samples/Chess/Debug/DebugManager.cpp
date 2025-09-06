//
// Created by Andrew Fagan on 1/2/25.
//

#include "DebugManager.h"

#include "Debug/ImGuiComponents/ChessboardComponents.h"
#include "Chessboard/ChessboardManager.h"
#include "ChessModule.h"

#include <imgui.h>


namespace Astral {

    void ChessDebugManager::Init()
    {
        PROFILE_SCOPE("ChessDebugManager::Init")
        m_RenderImGuiListener.StartListening();
    }


    void ChessDebugManager::Shutdown()
    {
        m_RenderImGuiListener.StopListening();
    }


    void ChessDebugManager::RenderImGui()
    {
        ImGui::Begin("Chess Debug Menu");

        Game::ChessboardManager& boardManager = ChessModule::Get().GetBoardManager();

        PieceMoveListTableComponent(boardManager.GetBoard(), boardManager.GetMoveList());

        ImGui::End();
    }

}
