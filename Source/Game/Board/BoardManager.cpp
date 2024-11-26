//
// Created by Andrew Fagan on 11/2/24.
//

#include "BoardManager.h"

namespace Game
{

    BoardManager& g_BoardManager = BoardManager::Get();


    BoardManager& BoardManager::Get()
    {
        static BoardManager m_Instance = BoardManager();
        return m_Instance;
    }


    void BoardManager::Init()
    {
        PROFILE_SCOPE();
        TRACE("Initializing Board Manager!")
        m_UpdateListener.StartListening();
        m_WhiteMoveList.GenerateMoves(m_Board, PieceColor::WHITE);
        m_BlackMoveList.GenerateMoves(m_Board, PieceColor::BLACK);
    }


    void BoardManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down Board Manager!")
        m_UpdateListener.StopListening();
    }


    void BoardManager::Update()
    {
        PROFILE_SCOPE();
    }

    BoardManager::BoardManager() : m_Board(Board()), m_BlackMoveList(m_Board, PieceColor::BLACK), m_WhiteMoveList(m_Board, PieceColor::WHITE)
    {
        PROFILE_SCOPE();
        TRACE("Constructing Board Manager!")
    }

    BoardManager::~BoardManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying Board Manager!")
    }


}

