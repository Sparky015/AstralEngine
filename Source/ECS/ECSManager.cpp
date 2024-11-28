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
        m_ECS.ReserveMemory();
        m_UpdateListener.StartListening();


        m_ECS.AddEntity();
    }

    void ECSManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down ECS Manager!")
        m_UpdateListener.StopListening();
        m_ECS.FreeMemory();
    }

    void ECSManager::Update()
    {
        PROFILE_SCOPE();
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