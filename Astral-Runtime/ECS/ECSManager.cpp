//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECSManager.h"

#include "ECS.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral {

    ECSManager& ECSManager::Get()
    {
        static ECSManager ecsManager;
        return ecsManager;
    }


    void ECSManager::Init()
    {
        PROFILE_SCOPE("ECS Manager Initialization");
        TRACE("Initializing ECS Manager!")
        GetECS().Init();
    }


    void ECSManager::Shutdown()
    {
        PROFILE_SCOPE("ECS Manager Shutdown");
        TRACE("Shutting down ECS Manager!")
        GetECS().Shutdown();
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