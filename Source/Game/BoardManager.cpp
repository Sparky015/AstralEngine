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

    }


    void BoardManager::Shutdown()
    {

    }


    void BoardManager::Update()
    {

    }

    BoardManager::BoardManager() : m_Board(Board())
    {
        LOG("Constructing Board Manager!")
    }

    BoardManager::~BoardManager()
    {
        LOG("Destroying Board Manager!")
    }


}

