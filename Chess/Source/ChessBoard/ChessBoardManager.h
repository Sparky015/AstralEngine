//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "ChessBoard.h"
#include "MoveGeneration/BoardMoveList.h"
#include "ECS/Entity.h"

namespace Game{


    class ChessBoardManager : public SystemManager
    {
    public:
        static ChessBoardManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline ChessBoard& GetBoard() {return m_Board; }
        [[nodiscard]] inline const BoardMoveList& GetMoveList() const {return m_MoveList; }

        ChessBoardManager(const ChessBoardManager&) = delete;
        ChessBoardManager& operator=(const ChessBoardManager&) = delete;
        ChessBoardManager(ChessBoardManager&&) = delete;
        ChessBoardManager& operator=(ChessBoardManager&&) = delete;
    private:
        ChessBoardManager();
        ~ChessBoardManager() override;

        void InitBoardEntities();
        void DestroyBoardEntities();

        ChessBoard m_Board;
        BoardMoveList m_MoveList;
    };

    extern ChessBoardManager& g_BoardManager;
}