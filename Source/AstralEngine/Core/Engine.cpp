/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "Input/InputManager.h"
#include "Window/WindowManager.h"
#include "Debug/ImGuiManager.h"
#include "ECS/ECSManager.h"
#include "Renderer/RendererManager.h"

#include "ApplicationModule.h"

Engine* Engine::m_Instance = nullptr;

Engine::Engine() :
    m_WindowClosedListener(Event::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){this->m_IsLoopRunning = false;}}),
    m_IsLoopRunning(true),
    m_ApplicationModule(Application::CreateApplicationModule())
{
    PROFILE_SCOPE();
    ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
    m_Instance = this;


    // This is the order that systems are called in for the SubSystemUpdateEvent
    Window::g_WindowManager.Init();
    IO::g_IOManager.Init();
    Debug::g_ImGuiManager.Init();
    ECS::g_ECSManager.Init();
    Graphics::g_RendererManager.Init();
    m_ApplicationModule->Init();

    m_WindowClosedListener.StartListening();
}


Engine::~Engine()
{
    PROFILE_SCOPE();
    m_WindowClosedListener.StopListening();

    m_ApplicationModule->Shutdown();
    Graphics::g_RendererManager.Shutdown();
    ECS::g_ECSManager.Shutdown();
    Debug::g_ImGuiManager.Shutdown();
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
        m_ApplicationModule->Update();

        Debug::ImGuiManager::ImGuiBeginFrame();
        m_RenderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
        Debug::ImGuiManager::ImGuiEndFrame();

        Window::g_WindowManager.SwapBuffers();
    }
}
