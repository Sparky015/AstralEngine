//
// Created by Andrew Fagan on 10/30/24.
//

#include "Core/SystemManager.h"

#pragma once

namespace IO{

    class InputManager : public SystemManager
    {
    public:
        static InputManager& Get();

        void Init() override;
        void Shutdown() override;
        void Update() override;


        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(InputManager&&) = delete;
    private:
        InputManager();
        ~InputManager() override;
    };

    extern InputManager& g_IOManager;

}
