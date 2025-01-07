/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "Input/InputManager.h"
#include "Window/WindowManager.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "ECS/ECSManager.h"
#include "Renderer/RendererManager.h"

#include "ApplicationModule.h"

Engine* Engine::m_Instance = nullptr;

Engine::Engine() :
    m_WindowClosedListener(Event::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){this->m_IsLoopRunning = false;}}),
    m_IsLoopRunning(true),
    m_ApplicationModule(Application::CreateApplicationModule())
{
    PROFILE_SCOPE("Engine Initialization");
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
    PROFILE_SCOPE("Engine Shutdown");
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
    PROFILE_SCOPE("Engine Runtime");
    while (m_IsLoopRunning)
    {
        PROFILE_SCOPE("Frame");
        m_NewFramePublisher.PublishEvent( NewFrameEvent() );

        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
        m_ApplicationModule->Update();

        Debug::ImGuiManager::ImGuiBeginFrame();
        m_RenderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
        Debug::ImGuiManager::ImGuiEndFrame();

        Window::g_WindowManager.SwapBuffers();
    }
}
