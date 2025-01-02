//
// Created by Andrew Fagan on 11/2/24.
//

#include "ChessboardManager.h"


namespace Game
{

    ChessboardManager& g_BoardManager = ChessboardManager::Get();


    ChessboardManager& ChessboardManager::Get()
    {
        static ChessboardManager m_Instance = ChessboardManager();
        return m_Instance;
    }


    void ChessboardManager::Init()
    {
        PROFILE_SCOPE();
        TRACE("Initializing Board Manager!")

        m_UpdateListener.StartListening();
        m_MoveList.UpdateMoveList(m_Board, PieceColor::WHITE);
    }


    void ChessboardManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down Board Manager!")
        m_UpdateListener.StopListening();
    }


    void ChessboardManager::Update()
    {
        PROFILE_SCOPE();
    }


    ChessboardManager::ChessboardManager() : m_Board(Chessboard()), //"5kr1/1p2qp2/1Qb1p1pp/p1ppn1R1/PrN1PBnP/1RP1bN2/1P2KPP1/5B2 w - - 3 23"
                                             m_MoveList(m_Board, PieceColor::WHITE)
    {
        PROFILE_SCOPE();
        TRACE("Constructing Board Manager!")
    }


    ChessboardManager::~ChessboardManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying Board Manager!")
    }


    void ChessboardManager::InitBoardEntities()
    {

    }


    void ChessboardManager::DestroyBoardEntities()
    {

    }


}

