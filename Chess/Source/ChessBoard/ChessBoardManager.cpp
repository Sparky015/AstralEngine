//
// Created by Andrew Fagan on 11/2/24.
//

#include "ChessBoardManager.h"

namespace Game
{

    ChessBoardManager& g_BoardManager = ChessBoardManager::Get();


    ChessBoardManager& ChessBoardManager::Get()
    {
        static ChessBoardManager m_Instance = ChessBoardManager();
        return m_Instance;
    }


    void ChessBoardManager::Init()
    {
        PROFILE_SCOPE();
        TRACE("Initializing Board Manager!")

        m_UpdateListener.StartListening();
        m_MoveList.UpdateMoveList(m_Board, PieceColor::WHITE);
    }


    void ChessBoardManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down Board Manager!")
        m_UpdateListener.StopListening();
    }


    void ChessBoardManager::Update()
    {
        PROFILE_SCOPE();
    }


    ChessBoardManager::ChessBoardManager() : m_Board(ChessBoard()), //"5kr1/1p2qp2/1Qb1p1pp/p1ppn1R1/PrN1PBnP/1RP1bN2/1P2KPP1/5B2 w - - 3 23"
        m_MoveList(m_Board, PieceColor::WHITE)
    {
        PROFILE_SCOPE();
        TRACE("Constructing Board Manager!")
    }


    ChessBoardManager::~ChessBoardManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying Board Manager!")
    }


    void ChessBoardManager::InitBoardEntities()
    {

    }


    void ChessBoardManager::DestroyBoardEntities()
    {

    }


}

