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

}

Engine::~Engine()
{

}

void Engine::Run()
{
    m_SystemUpdatePublisher.PublishEvent( SystemUpdateEvent() );
}



