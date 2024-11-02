//
// Created by Andrew Fagan on 10/31/24.
//
#include "SystemManager.h"

SystemManager::SystemManager() :
    m_UpdateListener([this](SystemUpdateEvent e){this->Update();})
{}


