/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Core/Engine.h"

#include "ApplicationModule.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "../ECS/ECSManager.h"
#include "Input/InputManager.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"
#include "Debug/MemoryTracking/MemoryTracker.h"
#include "cpuinfo.h"


#include "Debug/MemoryTracking/Serialization/SceneMetricsImporter.h"

Engine* Engine::m_Instance = nullptr;

Engine::Engine() :
    m_WindowClosedListener(Core::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){this->m_IsLoopRunning = false;}}),
    m_ApplicationModule(Application::CreateApplicationModule()),
    m_IsLoopRunning(true)
{
    PROFILE_SCOPE("Engine Initialization");
    ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
    m_Instance = this;

    Core::MemoryTracker::Get().Init();

    // This is the order that systems are called in for the SubSystemUpdateEvent

    Window::g_WindowManager.Init();
    IO::g_IOManager.Init();
    Debug::g_ImGuiManager.Init();
    Astral::g_ECSManager.Init();
    Graphics::g_RendererManager.Init();
    m_ApplicationModule->Init();

    cpuinfo_initialize();

    m_WindowClosedListener.StartListening();
}


Engine::~Engine()
{
    PROFILE_SCOPE("Engine Shutdown");
    m_WindowClosedListener.StopListening();

    m_ApplicationModule->Shutdown();
    Graphics::g_RendererManager.Shutdown();
    Astral::g_ECSManager.Shutdown();
    Debug::g_ImGuiManager.Shutdown();
    IO::g_IOManager.Shutdown();
    Window::g_WindowManager.Shutdown();

    Core::MemoryTracker::Get().Shutdown();
}


void Engine::Run()
{
    PROFILE_SCOPE("Engine Runtime");

    Core::DeltaTime m_DeltaTime;

    while (m_IsLoopRunning)
    {
        PROFILE_SCOPE("Frame");

        m_DeltaTime.UpdateDeltaTime();

        m_NewFramePublisher.PublishEvent( NewFrameEvent() );

        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
        m_ApplicationModule->Update(m_DeltaTime);

        Debug::ImGuiManager::Get().BeginFrame();
        m_RenderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
        Debug::ImGuiManager::Get().EndFrame();

        Window::g_WindowManager.SwapBuffers();
    }
}
