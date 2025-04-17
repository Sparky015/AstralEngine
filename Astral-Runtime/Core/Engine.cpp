/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Core/Engine.h"

#include "ApplicationModule.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "ECS/ECSManager.h"
#include "Input/InputManager.h"
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

    // This is the order that systems are called in for the SubSystemUpdateEvent

    m_WindowManager.Init();
    IO::g_IOManager.Init();
    m_ImGuiManager.Init();
    m_ECSManager.Init();
    m_ApplicationModule->Init();

    cpuinfo_initialize();

    m_WindowClosedListener.StartListening();

}


Engine::~Engine()
{
    PROFILE_SCOPE("Engine Shutdown");
    m_WindowClosedListener.StopListening();

    m_ApplicationModule->Shutdown();
    m_ECSManager.Shutdown();
    m_ImGuiManager.Shutdown();
    IO::g_IOManager.Shutdown();
    m_WindowManager.Shutdown();


    Core::MemoryTracker::Get().Shutdown();
}


void Engine::Run()
{
    PROFILE_SCOPE("Engine Runtime");

    Core::DeltaTime m_DeltaTime;

    static int FramesToRun = 10;

    while (m_IsLoopRunning /*&& FramesToRun > 0*/)
    {
        PROFILE_SCOPE("Frame");

        m_DeltaTime.UpdateDeltaTime();

        m_NewFramePublisher.PublishEvent( NewFrameEvent() );

        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
        m_ApplicationModule->Update(m_DeltaTime);

        Debug::ImGuiManager::Get().BeginFrame();
        m_RenderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
        Debug::ImGuiManager::Get().EndFrame();

        m_WindowManager.SwapBuffers();

        // FramesToRun--;
    }
}
