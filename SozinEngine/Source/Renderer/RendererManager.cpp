//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

namespace Renderer {


    RendererManager& g_RendererManager = RendererManager::Get();

    RendererManager& RendererManager::Get()
    {
        static RendererManager s_RendererManager;
        return s_RendererManager;
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE();

        TRACE("Initializing Renderer Manager!");
        m_UpdateListener.StartListening();

    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE();

        TRACE("Shutting down Renderer Manager!")
        m_UpdateListener.StopListening();
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