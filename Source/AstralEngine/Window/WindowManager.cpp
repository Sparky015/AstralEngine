//
// Created by Andrew Fagan on 10/31/24.
//

#include "WindowManager.h"

namespace Window{

    WindowManager& g_WindowManager = WindowManager::Get();

    void WindowManager::Update()
    {
        PROFILE_SCOPE();
        m_Window->Update();
    }


    WindowManager::WindowManager() : m_Window{nullptr}
    {
        PROFILE_SCOPE();
        TRACE("Constructing Window System!")
    }


    WindowManager::~WindowManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying Window System!")
    }


    WindowManager& WindowManager::Get()
    {
        static WindowManager m_Instance = WindowManager();
        return m_Instance;
    }


    void WindowManager::Init()
    {
        PROFILE_SCOPE();
        TRACE("Initializing Window Manager!")

        m_UpdateListener.StartListening();

        m_Window.reset( Window::CreateWindow());
        m_Window->Init();
    }


    void WindowManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down Window Manager!")
        m_UpdateListener.StopListening();
        m_RenderImGuiListener.StopListening();
        m_Window->Shutdown();
    }

}
