/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "IO/IOManager.h"
#include "Window/WindowManager.h"
#include "Debug/DebugManager.h"
#include "Game/Board/BoardManager.h"
#include "ECS/ECSManager.h"
#include "Game/GameManager.h"


Engine* Engine::m_Instance = nullptr;

Engine::Engine() :
    m_IsLoopRunning(true),
    m_WindowClosedListener(Event::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){this->m_IsLoopRunning = false;}})
{
    PROFILE_SCOPE();
    ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
    m_Instance = this;

    // This is the order that systems are called in for the SubSystemUpdateEvent
    Window::g_WindowManager.Init();
    IO::g_IOManager.Init();
    Debug::g_DebugManager.Init();
    ECS::g_ECSManager.Init();
    Game::g_BoardManager.Init();
    Game::g_GameManager.Init();

    m_WindowClosedListener.StartListening();
}


Engine::~Engine()
{
    PROFILE_SCOPE();
    m_WindowClosedListener.StopListening();

    Game::g_GameManager.Shutdown();
    Game::g_BoardManager.Shutdown();
    ECS::g_ECSManager.Shutdown();
    Debug::g_DebugManager.Shutdown();
    IO::g_IOManager.Shutdown();
    Window::g_WindowManager.Shutdown();
}


void Engine::Run()
{
    PROFILE_SCOPE();
    while (m_IsLoopRunning)
    {
        PROFILE_SCOPE();

        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );

        Debug::DebugManager::ImGuiBegin();
        m_RenderImGuiPublisher.PublishEvent(RenderImGuiEvent() );
        Debug::DebugManager::ImGuiEnd();

        Window::g_WindowManager.SwapBuffers();
    }
}
