//
// Created by Andrew Fagan on 10/30/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "InputState.h"


namespace IO {

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

        InputState m_InputState;
    };

    extern InputManager& g_IOManager;

}
