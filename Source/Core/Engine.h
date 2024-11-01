/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "Core/Events/EventPublisher.h"

#include "IOManager/IOManager.h"
#include "WindowManager/WindowManager.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

private:
    Event::EventPublisher<SystemUpdateEvent> m_SystemUpdatePublisher;

    IOManager m_IOManager;
    WindowManager m_WindowManager;


};