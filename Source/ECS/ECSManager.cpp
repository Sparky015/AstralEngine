//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECSManager.h"

namespace ECS {

    ECSManager& g_ECSManager = ECSManager::Get();

    ECSManager& ECSManager::Get()
    {
        static ECSManager ecsManager;
        return ecsManager;
    }

    void ECSManager::Init()
    {
        PROFILE_SCOPE();
        TRACE("Initializing ECS Manager!")
        m_UpdateListener.StartListening();

        m_ECS.AddEntity();
    }

    void ECSManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down ECS Manager!")
        m_UpdateListener.StopListening();
    }

    void ECSManager::Update()
    {
        PROFILE_SCOPE();
        Entity e = m_ECS.AddEntity();
        m_ECS.AddComponent(e, TransformComponent(2,2));
        m_ECS.GetComponent<TransformComponent>(e);
    }

    ECSManager::ECSManager()
    {
        PROFILE_SCOPE();
        TRACE("Constructing ECS Manager!")
    }

    ECSManager::~ECSManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying ECS Manager!")
    }

} // ECS