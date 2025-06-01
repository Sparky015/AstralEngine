//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECSManager.h"

#include "ECS.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "Systems/RenderingSystem.h"

namespace Astral {

    ECSManager::ECSManager() :
        m_ECS(nullptr)
    {
        TRACE("Constructing ECS Manager!")
    }


    ECSManager::~ECSManager()
    {
        TRACE("Destroying ECS Manager!")
    }


    void ECSManager::Init()
    {
        PROFILE_SCOPE("ECS Manager Initialization");
        TRACE("Initializing ECS Manager!")
        m_ECS = CreateScopedPtr<ECS>();
        GetECS().Init();
        m_UpdateListener.StartListening();
    }


    void ECSManager::Update()
    {
        PROFILE_SCOPE("ECS Systems");
        RenderingSystem::RenderEntities();
    }


    void ECSManager::Shutdown()
    {
        PROFILE_SCOPE("ECS Manager Shutdown");
        TRACE("Shutting down ECS Manager!")
        m_UpdateListener.StopListening();
        GetECS().Shutdown();
        m_ECS.reset();
    }

} // ECS