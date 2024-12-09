//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Board.h"
#include "Game/MoveList.h"
#include "ECS/Entity.h"

namespace Game{


    class BoardManager : public SystemManager
    {
    public:
        static BoardManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline Board& GetBoard() {return m_Board; }
        [[nodiscard]] inline MoveList& GetMoveList(PieceColor color) {return (color == PieceColor::WHITE) ? m_WhiteMoveList : m_BlackMoveList; }

        BoardManager(const BoardManager&) = delete;
        BoardManager& operator=(const BoardManager&) = delete;
        BoardManager(BoardManager&&) = delete;
        BoardManager& operator=(BoardManager&&) = delete;
    private:
        BoardManager();
        ~BoardManager() override;

        void InitBoardEntities();
        void DestroyBoardEntities();

        Board m_Board;
        MoveList m_WhiteMoveList;
        MoveList m_BlackMoveList;
    };

    extern BoardManager& g_BoardManager;
}