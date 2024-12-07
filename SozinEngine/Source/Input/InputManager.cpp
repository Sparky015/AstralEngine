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
        PROFILE_SCOPE();
        TRACE("Initializing IO Manager!")
        m_UpdateListener.StartListening();
    }


    void InputManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down IO Manager!")
        m_UpdateListener.StopListening();
    }


    void InputManager::Update()
    {
        PROFILE_SCOPE();
    }


    InputManager::InputManager()
    {
        PROFILE_SCOPE();
        TRACE("Constructing IO System!")
    }

    InputManager::~InputManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying IO System!")
    }

}
