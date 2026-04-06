/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "ApplicationModule.h"
#include "Debug/MemoryTracking/MemoryTracker.h"
#include "Debug/MemoryTracking/Serialization/SceneMetricsImporter.h"
#include "Debug/ImGui/ImGuiEvents.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Scenes/SceneManager.h"
#include "Events/EventListener.h"
#include "Events/EventPublisher.h"
#include "EngineLoopEvents.h"
#include "Input/InputState.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowEvents.h"
#include "Window/WindowManager.h"

#include "cpuinfo.h"
#include "glslang/Include/glslang_c_interface.h"
#include "nfd.hpp"
#include "Asset/AssetManager.h"

namespace Astral {

    Engine* Engine::m_Instance = nullptr;

    Engine::Engine() :
        m_ApplicationModule(Application::CreateApplicationModule()),
        m_IsLoopRunning(true),
        m_FrameCount(0),

        m_WindowManager(CreateScopedPtr<WindowManager>()),
        m_ImGuiManager(CreateScopedPtr<ImGuiManager>()),
        m_RendererManager(CreateScopedPtr<RendererManager>()),
        m_AssetManager(CreateScopedPtr<AssetManager>()),
        m_ECSManager(CreateScopedPtr<SceneManager>()),
        m_JobManager(CreateScopedPtr<JobManager>())
    {
        PROFILE_SCOPE("Engine::Engine");
        ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
        m_Instance = this;

        Astral::MemoryTracker::Get().Init();

        // This is the order that systems are called in for the SubSystemUpdateEvent
        m_WindowManager->Init();
        m_ImGuiManager->Init();
        m_AssetManager->Init();
        m_RendererManager->Init();
        m_AssetManager->InitAssetLoaderDefaults();
        InputState::Init();
        m_ECSManager->Init();
        m_JobManager->Init();
        m_ApplicationModule->Init();


        AE_PROFILE_FUNCTION(cpuinfo_initialize(), "cpuinfo_initialize");
        AE_PROFILE_FUNCTION(glslang_initialize_process(), "glslang_initialize_process");
        AE_PROFILE_FUNCTION(NFD_Init(), "NFD_Init");
    }


    Engine::~Engine()
    {
        PROFILE_SCOPE("Engine::~Engine");

        AE_PROFILE_FUNCTION(NFD_Quit(), "NFD_Quit");
        AE_PROFILE_FUNCTION(glslang_finalize_process(), "glslang_finalize_process");
        AE_PROFILE_FUNCTION(cpuinfo_deinitialize(), "cpuinfo_deinitialize");

        m_ApplicationModule->Shutdown();
        m_JobManager->Shutdown();
        m_JobManager.reset();
        m_ECSManager->Shutdown();
        m_ECSManager.reset();
        InputState::Shutdown();
        m_AssetManager->Shutdown();
        m_AssetManager.reset();
        m_RendererManager->Shutdown();
        m_RendererManager.reset();
        m_ImGuiManager->Shutdown();
        m_ImGuiManager.reset();
        m_WindowManager->Shutdown();
        m_WindowManager.reset();

        Astral::MemoryTracker::Get().Shutdown();
    }


    void Engine::Run()
    {
        PROFILE_SCOPE("Engine::Run");

        Astral::DeltaTime m_DeltaTime;
        Astral::EventPublisher<SubSystemUpdateEvent> subSystemUpdatePublisher;
        Astral::EventPublisher<NewFrameEvent> newFramePublisher;
        Astral::EventPublisher<RenderImGuiEvent> renderImGuiPublisher;
        Astral::EventListener<WindowClosedEvent> windowClosedListener{
            Astral::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){ this->m_IsLoopRunning = false; }}
        };
        windowClosedListener.StartListening();

        char frameScopeProfilerNameBuffer[64];

                                  /****  Engine Loop  ****/
        while (m_IsLoopRunning)
        {
            snprintf(frameScopeProfilerNameBuffer, sizeof(frameScopeProfilerNameBuffer), "Frame %zu", m_FrameCount);
            PROFILE_SCOPE(frameScopeProfilerNameBuffer);

            m_DeltaTime.UpdateDeltaTime();

            newFramePublisher.PublishEvent( NewFrameEvent() );

            m_ImGuiManager->BeginFrame();
            renderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
            m_ImGuiManager->EndFrame();

            subSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
            m_ApplicationModule->Update(m_DeltaTime);


            m_WindowManager->SwapBuffers();

            m_FrameCount++;
        }


        windowClosedListener.StopListening();
    }

}
