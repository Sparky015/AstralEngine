//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Debug/Instrumentation/Public/ScopeProfiler.h"
#include "Debug/Utils/Public/Loggers.h"

namespace Graphics {

    RendererManager& g_RendererManager = RendererManager::Get();

    RendererManager& RendererManager::Get()
    {
        static RendererManager s_RendererManager;
        return s_RendererManager;
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE("Renderer Manager Initialization");

        TRACE("Initializing Renderer Manager!");
        // m_UpdateListener.StartListening();
    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE("Renderer Manager Update");
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("Renderer Manager Shutdown");

        TRACE("Shutting down Renderer Manager!")
        // m_UpdateListener.StopListening();
    }


    RendererManager::RendererManager()
    {
        TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        TRACE("Destroying Renderer Manager!")
    }

}
