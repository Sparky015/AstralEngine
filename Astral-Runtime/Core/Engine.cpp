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
#include "ECS/SceneManager.h"
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

        m_WindowManager(CreateScopedPtr<WindowManager>()),
        m_ImGuiManager(CreateScopedPtr<ImGuiManager>()),
        m_RendererManager(CreateScopedPtr<RendererManager>()),
        m_AssetManager(CreateScopedPtr<AssetManager>()),
        m_ECSManager(CreateScopedPtr<SceneManager>())

    {
        PROFILE_SCOPE("Engine Initialization");
        ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
        m_Instance = this;

        Astral::MemoryTracker::Get().Init();

        // This is the order that systems are called in for the SubSystemUpdateEvent
        m_WindowManager->Init();
        m_ImGuiManager->Init();
        m_AssetManager->Init();
        m_RendererManager->Init();
        InputState::Init();
        m_ECSManager->Init();
        m_ApplicationModule->Init();

        cpuinfo_initialize();
        glslang_initialize_process();
        NFD_Init();
    }


    Engine::~Engine()
    {
        PROFILE_SCOPE("Engine Shutdown");

        NFD_Quit();
        glslang_finalize_process();
        cpuinfo_deinitialize();

        m_ApplicationModule->Shutdown();
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
        PROFILE_SCOPE("Engine Runtime");

        Astral::DeltaTime m_DeltaTime;
        Astral::EventPublisher<SubSystemUpdateEvent> subSystemUpdatePublisher;
        Astral::EventPublisher<NewFrameEvent> newFramePublisher;
        Astral::EventPublisher<RenderImGuiEvent> renderImGuiPublisher;
        Astral::EventListener<WindowClosedEvent> windowClosedListener{
            Astral::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){ this->m_IsLoopRunning = false; }}
        };
        windowClosedListener.StartListening();


                                  /****  Engine Loop  ****/
        while (m_IsLoopRunning)
        {
            PROFILE_SCOPE("Frame");

            m_DeltaTime.UpdateDeltaTime();

            newFramePublisher.PublishEvent( NewFrameEvent() );

            m_ImGuiManager->BeginFrame();
            renderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
            m_ImGuiManager->EndFrame();

            subSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
            m_ApplicationModule->Update(m_DeltaTime);



            m_WindowManager->SwapBuffers();
        }


        windowClosedListener.StopListening();
    }

}
