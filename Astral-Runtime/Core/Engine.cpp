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
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"
#include "Debug/MemoryTracking/MemoryTracker.h"
#include "Debug/MemoryTracking/Serialization/SceneMetricsImporter.h"

#include "cpuinfo.h"
#include "glslang/Include/glslang_c_interface.h"


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
    // Core::MemoryTracker::Get().BeginScene("Engine_Init");

    // This is the order that systems are called in for the SubSystemUpdateEvent
    Window::g_WindowManager.Init();
    Debug::g_ImGuiManager.Init();
    Astral::g_RendererManager.Init();
    IO::g_IOManager.Init();
    ECS::g_ECSManager.Init();

    m_ApplicationModule->Init();

    cpuinfo_initialize();
    glslang_initialize_process();


    m_WindowClosedListener.StartListening();
    // Core::MemoryTracker::Get().EndScene();
}


Engine::~Engine()
{
    PROFILE_SCOPE("Engine Shutdown");
    m_WindowClosedListener.StopListening();

    glslang_finalize_process();
    cpuinfo_deinitialize();

    m_ApplicationModule->Shutdown();
    ECS::g_ECSManager.Shutdown();
    IO::g_IOManager.Shutdown();
    Astral::g_RendererManager.Shutdown();
    Debug::g_ImGuiManager.Shutdown();
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
        m_ApplicationModule->Update(m_DeltaTime);


        Debug::ImGuiManager::Get().BeginFrame();
        m_RenderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
        Debug::ImGuiManager::Get().EndFrame();

        m_SubSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );


        Window::g_WindowManager.SwapBuffers();
    }
}
