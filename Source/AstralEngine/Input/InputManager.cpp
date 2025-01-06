//
// Created by Andrew Fagan on 10/30/24.
//

#include "InputManager.h"

namespace IO{

    InputManager& g_IOManager = InputManager::Get();

    InputManager& InputManager::Get()
    {
        static InputManager m_Instance = InputManager();
        return m_Instance;
    }


    void InputManager::Init()
    {
        PROFILE_SCOPE("Input State Initialization");
        TRACE("Initializing IO Manager!")
        InputState::Init();
        // m_UpdateListener.StartListening();
    }


    void InputManager::Shutdown()
    {
        PROFILE_SCOPE("Input State Shutdown");
        TRACE("Shutting down IO Manager!")
        InputState::Shutdown();
        // m_UpdateListener.StopListening();
    }


    void InputManager::Update()
    {

    }


    InputManager::InputManager()
    {
        TRACE("Constructing IO System!")
    }

    InputManager::~InputManager()
    {
        TRACE("Destroying IO System!")
    }

}
