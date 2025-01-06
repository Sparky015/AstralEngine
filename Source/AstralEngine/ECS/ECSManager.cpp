//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECSManager.h"

#include "Systems/RenderingSystem.h"

namespace ECS {

    ECSManager& g_ECSManager = ECSManager::Get();

    ECSManager& ECSManager::Get()
    {
        static ECSManager ecsManager;
        return ecsManager;
    }

    void ECSManager::Init()
    {
        PROFILE_SCOPE("ECS Manager Initialization");
        TRACE("Initializing ECS Manager!")
        // m_UpdateListener.StartListening();

    }

    void ECSManager::Shutdown()
    {
        PROFILE_SCOPE("ECS Manager Shutdown");
        TRACE("Shutting down ECS Manager!")
        // m_UpdateListener.StopListening();
    }

    void ECSManager::Update()
    {
        PROFILE_SCOPE("ECS Manager Update");
        // Sozin::RenderingSystem::RenderEntities(m_ShaderProgram.get());
    }

    ECSManager::ECSManager()
    {
        TRACE("Constructing ECS Manager!")
    }

    ECSManager::~ECSManager()
    {
        TRACE("Destroying ECS Manager!")
    }

} // ECS