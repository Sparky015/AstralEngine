/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "Core/Events/EventPublisher.h"
#include "Core/Events/EventListener.h"
#include "Window/WindowEvents.h"
#include "Debug/ImGui/ImGuiEvents.h"
#include "SystemEvents.h"
#include "Renderer/RendererEvents.h"
#include "ApplicationModule.h"


class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

   static inline Engine& Get() { return *m_Instance; }

private:
    Core::EventPublisher<SubSystemUpdateEvent> m_SubSystemUpdatePublisher;
    Core::EventPublisher<NewFrameEvent> m_NewFramePublisher;
    Core::EventPublisher<RenderImGuiEvent> m_RenderImGuiPublisher;
    Core::EventListener<WindowClosedEvent> m_WindowClosedListener;

    bool m_IsLoopRunning;

    static Engine* m_Instance;
    Application::ApplicationModule* m_ApplicationModule;
};