/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "ApplicationModule.h"
#include "Core/Events/EventListener.h"
#include "Core/Events/EventPublisher.h"
#include "Core/Time/Clock.h"
#include "Core/Time/DeltaTime.h"
#include "Debug/ImGui/ImGuiEvents.h"
#include "Renderer/RendererEvents.h"
#include "SystemEvents.h"
#include "Window/WindowEvents.h"


class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

   static inline Engine& Get() { return *m_Instance; }

   /**@brief Gets the amount of time in seconds since the engine was initialized in seconds. */
   [[nodiscard]] float GetEngineTime() const { return m_Clock.GetTime(); };

private:
    Core::EventPublisher<SubSystemUpdateEvent> m_SubSystemUpdatePublisher;
    Core::EventPublisher<NewFrameEvent> m_NewFramePublisher;
    Core::EventPublisher<RenderImGuiEvent> m_RenderImGuiPublisher;
    Core::EventListener<WindowClosedEvent> m_WindowClosedListener;

    static Engine* m_Instance;
    Application::ApplicationModule* m_ApplicationModule;
    bool m_IsLoopRunning;
    Core::DeltaTime m_DeltaTime;
    Core::Clock m_Clock;
};