/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "ApplicationModule.h"
#include "Debug/MemoryTracking/MemoryTracker.h"
#include "cpuinfo.h"
#include "nfd.hpp"

#include "Events/EventListener.h"
#include "Events/EventPublisher.h"
#include "Debug/ImGui/ImGuiEvents.h"
#include "EngineLoopEvents.h"
#include "Window/WindowEvents.h"

#include "Window/WindowManager.h"
#include "ECS/ECSManager.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Input/InputState.h"


namespace Astral {

    Engine* Engine::m_Instance = nullptr;

    Engine::Engine() :
        m_ApplicationModule(Application::CreateApplicationModule()),
        m_IsLoopRunning(true),

        m_WindowManager(CreateScopedPtr<WindowManager>()),
        m_ECSManager(CreateScopedPtr<ECSManager>()),
        m_ImGuiManager(CreateScopedPtr<Debug::ImGuiManager>())
    {
        PROFILE_SCOPE("Engine Initialization");
        ASSERT(m_Instance == nullptr, "Engine has already been initialized!");
        m_Instance = this;

        // This is the order that systems are called in for the SubSystemUpdateEvent
        m_WindowManager->Init();
        InputState::Init();
        m_ImGuiManager->Init();
        m_ECSManager->Init();
        m_ApplicationModule->Init();

        cpuinfo_initialize();
        NFD_Init();
    }


    Engine::~Engine()
    {
        PROFILE_SCOPE("Engine Shutdown");

        NFD_Quit();
        cpuinfo_deinitialize();

        m_ApplicationModule->Shutdown();
        m_ECSManager->Shutdown();
        m_ImGuiManager->Shutdown();
        InputState::Shutdown();
        m_WindowManager->Shutdown();

        Core::MemoryTracker::Get().Shutdown();
    }


    void Engine::Run()
    {
        PROFILE_SCOPE("Engine Runtime");

        Core::DeltaTime m_DeltaTime;
        Core::EventPublisher<SubSystemUpdateEvent> subSystemUpdatePublisher;
        Core::EventPublisher<NewFrameEvent> newFramePublisher;
        Core::EventPublisher<RenderImGuiEvent> renderImGuiPublisher;
        Core::EventListener<WindowClosedEvent> windowClosedListener{
            Core::EventListener<WindowClosedEvent>{[this](WindowClosedEvent e){ this->m_IsLoopRunning = false; }}
        };
        windowClosedListener.StartListening();


                                  /****  Engine Loop  ****/
        while (m_IsLoopRunning)
        {
            PROFILE_SCOPE("Frame");

            m_DeltaTime.UpdateDeltaTime();

            newFramePublisher.PublishEvent( NewFrameEvent() );

            subSystemUpdatePublisher.PublishEvent( SubSystemUpdateEvent() );
            m_ApplicationModule->Update(m_DeltaTime);

            Debug::ImGuiManager::Get().BeginFrame();
            renderImGuiPublisher.PublishEvent( RenderImGuiEvent() );
            Debug::ImGuiManager::Get().EndFrame();

            m_WindowManager->SwapBuffers();
        }


        windowClosedListener.StopListening();
    }

}
