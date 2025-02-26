//
// Created by Andrew Fagan on 10/31/24.
//

#include "WindowManager.h"

#include "../Debug/Instrumentation/ScopeProfiler.h"
#include "../Debug/Macros/Loggers.h"

namespace Window{

    WindowManager& g_WindowManager = WindowManager::Get();

    void WindowManager::Update()
    {
        m_Window->Update();
    }


    WindowManager::WindowManager() : m_Window{nullptr}
    {
        TRACE("Constructing Window System!")
    }


    WindowManager::~WindowManager()
    {
        TRACE("Destroying Window System!")
    }


    WindowManager& WindowManager::Get()
    {
        static WindowManager m_Instance = WindowManager();
        return m_Instance;
    }


    void WindowManager::Init()
    {
        PROFILE_SCOPE("MemorySceneMetricsImporter")
        TRACE("Initializing Window Manager!")

        m_UpdateListener.StartListening();

        m_Window.reset( Window::CreateWindow());
        m_Window->Init();
    }


    void WindowManager::Shutdown()
    {
        TRACE("Shutting down Window Manager!")
        m_UpdateListener.StopListening();
        m_RenderImGuiListener.StopListening();
        m_Window->Shutdown();
    }

}
