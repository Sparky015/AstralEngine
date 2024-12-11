//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "ECS/ECSManager.h"
#include "Board/BoardManager.h"

namespace Game {

    GameManager& g_GameManager = GameManager::Get();

    GameManager& GameManager::Get()
    {
        static GameManager s_GameManager;
        return s_GameManager;
    }


    void GameManager::Init()
    {
        TRACE("Initializing Game Manager");
        m_UpdateListener.StartListening();

        Board& board = g_BoardManager.GetBoard();

        // Create an entity for each piece

//        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
//        {
//
//        }
    }


    void GameManager::Update()
    {

    }


    void GameManager::Shutdown()
    {
        TRACE("Shutting down Game Manager");
        m_UpdateListener.StopListening();
    }


    GameManager::GameManager()
    {
        TRACE("Constructing Game Manager");
    }


    GameManager::~GameManager()
    {
        TRACE("Destroying Game Manager");
    }



}