/**
* @file Engine.cpp
* @author Andrew Fagan
* @date 10/31/2024
*/
#include "Engine.h"

#include "IO/IOManager.h"

Engine::Engine()
{
    m_IOManager.Setup();


    
}

Engine::~Engine()
{
    m_IOManager.Shutdown();
}

void Engine::Run() const
{

}



