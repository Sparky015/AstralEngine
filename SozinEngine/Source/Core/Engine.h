/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "Core/Events/EventPublisher.h"
#include "Core/Events/EventListener.h"

#include "Window/WindowEvents.h"
#include "Debug/DebugEvents.h"
#include "SystemEvents.h"



class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

   static inline Engine& Get() { return *m_Instance; }

private:
    Event::EventPublisher<SubSystemUpdateEvent> m_SubSystemUpdatePublisher;
    Event::EventPublisher<RenderImGuiEvent> m_RenderImGuiPublisher;
    Event::EventListener<WindowClosedEvent> m_WindowClosedListener;

    bool m_IsLoopRunning;

    static Engine* m_Instance;
};