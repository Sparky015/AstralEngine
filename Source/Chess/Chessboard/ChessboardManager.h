//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Chessboard.h"
#include "../MoveGeneration/BoardMoveList.h"
#include "ECS/Entity.h"

namespace Game{


    class ChessboardManager : public SystemManager
    {
    public:
        static ChessboardManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline Chessboard& GetBoard() {return m_Board; }
        [[nodiscard]] inline BoardMoveList& GetMoveList() {return m_MoveList; }

        ChessboardManager(const ChessboardManager&) = delete;
        ChessboardManager& operator=(const ChessboardManager&) = delete;
        ChessboardManager(ChessboardManager&&) = delete;
        ChessboardManager& operator=(ChessboardManager&&) = delete;
    private:
        ChessboardManager();
        ~ChessboardManager() override;

        void InitBoardEntities() const;
        void DestroyBoardEntities() const;

        Chessboard m_Board;
        BoardMoveList m_MoveList;
    };

    extern ChessboardManager& g_BoardManager;
}