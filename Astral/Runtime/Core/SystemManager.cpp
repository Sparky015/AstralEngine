//
// Created by Andrew Fagan on 10/31/24.
//
#include "SystemManager.h"

SystemManager::SystemManager() :
    m_RenderImGuiListener([this](RenderImGuiEvent e){this->RenderImGui();})
{}


void SystemManager::Update()
{

}


void SystemManager::RenderImGui()
{

}


void SystemManager::EnableImGuiRenderingCallback()
{
    m_RenderImGuiListener.StartListening();
}


void SystemManager::DisableImGuiRenderingCallback()
{
    m_RenderImGuiListener.StopListening();
}


