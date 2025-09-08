//
// Created by Andrew Fagan on 10/31/24.
//

#include "WindowManager.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral{

    void WindowManager::Update()
    {
        m_Window->Update();
    }


    WindowManager::WindowManager() : m_Window{nullptr}
    {
        AE_TRACE("Constructing Window System!")
    }


    WindowManager::~WindowManager()
    {
        AE_TRACE("Destroying Window System!")
    }


    WindowManager& WindowManager::Get()
    {
        static WindowManager m_Instance = WindowManager();
        return m_Instance;
    }


    void WindowManager::Init()
    {
        PROFILE_SCOPE("WindowManager::Init")
        AE_TRACE("Initializing Window Manager!")

        m_UpdateListener.StartListening();

        m_Window.reset( Window::ConstructWindow());
        m_Window->Init();
    }


    void WindowManager::Shutdown()
    {
        AE_TRACE("Shutting down Window Manager!")
        m_UpdateListener.StopListening();
        m_RenderImGuiListener.StopListening();
        m_Window->Shutdown();
    }

}
