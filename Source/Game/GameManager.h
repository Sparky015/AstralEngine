//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"

namespace Game {

    class GameManager : SystemManager
    {
    public:
        static GameManager& Get();
        void Init() override;
        void Update() override;
        void Shutdown() override;

        GameManager(const GameManager&) = delete;
        GameManager operator=(const GameManager&) = delete;
        GameManager(GameManager&&) = delete;
        GameManager operator=(GameManager&&) = delete;
    private:
        GameManager();
        ~GameManager() override;
    };

    extern GameManager& g_GameManager;

}