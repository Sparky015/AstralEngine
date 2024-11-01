/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "IOManager/IOManager.h"
#include "Core/Events/EventListener.h"
#include "Core/Events/EventPublisher.h"

Engine::Engine()
{
    m_IOManager.Setup();
}

Engine::~Engine()
{
    m_IOManager.Shutdown();
}

void Engine::Run()
{
    using namespace Event;

    EventPublisher<int&> pub;
    EventListener<int&> listener = EventListener<int&>([](int& i){LOG(i)});
    int f = 5;
    pub.PublishEvent(f);

    SystemUpdateEvent sue = SystemUpdateEvent();
    m_SystemUpdatePublisher.PublishEvent(sue);
}



