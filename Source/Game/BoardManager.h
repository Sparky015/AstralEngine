//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Game/Board.h"

namespace Game{


    class BoardManager : public SystemManager
    {
    public:
        static BoardManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline Board& GetBoard() {return m_Board; }
    private:
        BoardManager();
        ~BoardManager() override = default;

        Board m_Board;
    };

    extern BoardManager& g_BoardManager;
}