//
// Created by Andrew Fagan on 10/31/24.
//
#include "SystemManager.h"

SystemManager::SystemManager() :
    m_UpdateListener([this](SubSystemUpdateEvent e){this->Update();}),
    m_RenderImGuiListener([this](RenderImGuiEvent e){this->RenderImGui();})
{}


void SystemManager::Update()
{

}

void SystemManager::RenderImGui()
{

}


