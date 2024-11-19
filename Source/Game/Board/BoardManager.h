//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Board.h"

namespace Game{


    class BoardManager : public SystemManager
    {
    public:
        static BoardManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline Board& GetBoard() {return m_Board; }

        BoardManager(const BoardManager&) = delete;
        BoardManager& operator=(const BoardManager&) = delete;
        BoardManager(BoardManager&&) = delete;
        BoardManager& operator=(BoardManager&&) = delete;
    private:
        BoardManager();
        ~BoardManager() override;

        Board m_Board;
    };

    extern BoardManager& g_BoardManager;
}