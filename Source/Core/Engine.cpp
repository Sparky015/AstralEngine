/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "IO/IOManager.h"
#include "Window/WindowManager.h"
#include "Debug/DebugManager.h"
#include "Game/BoardManager.h"


Engine* Engine::m_Instance = nullptr;

Engine::Engine() :
    m_IsLoopRunning(true),
    m_WindowClosedListener(Event::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){this->m_IsLoopRunning = false;}})
{
    ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
    m_Instance = this;

    Window::g_WindowManager.Init();
    IO::g_IOManager.Init();
    Debug::g_DebugManager.Init();
    Game::g_BoardManager.Init();

    m_WindowClosedListener.StartListening();
}


Engine::~Engine()
{
    m_WindowClosedListener.StopListening();

    Game::g_BoardManager.Shutdown();
    Debug::g_DebugManager.Shutdown();
    IO::g_IOManager.Shutdown();
    Window::g_WindowManager.Shutdown();
}


void Engine::Run()
{
    while (m_IsLoopRunning)
    {
        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );

        Debug::DebugManager::ImGuiBegin();
        m_RenderImGuiPublisher.PublishEvent(RenderImGuiEvent() );
        Debug::DebugManager::ImGuiEnd();
    }
}





