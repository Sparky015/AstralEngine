//
// Created by Andrew Fagan on 1/2/25.
//

#include "DebugManager.h"

#include "Debug/ImGuiComponents/ChessboardComponents.h"

#include <imgui.h>

#include "Chessboard/ChessboardManager.h"

namespace Debug {

    DebugManager& g_DebugManager = DebugManager::Get();

    DebugManager& DebugManager::Get()
    {
        static DebugManager m_Instance = DebugManager();
        return m_Instance;
    }


    void DebugManager::Init()
    {
        m_RenderImGuiListener.StartListening();
    }

    void DebugManager::Shutdown()
    {
        m_RenderImGuiListener.StopListening();
    }


    void DebugManager::RenderImGui()
    {
        ImGui::Begin("Chess Debug Menu");

        PieceMoveListTableComponent(Game::g_BoardManager.GetBoard(), Game::g_BoardManager.GetMoveList());

        ImGui::End();
    }

}
