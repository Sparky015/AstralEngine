//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "RHI/RendererAPI.h"

namespace Astral {

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
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();
        m_UpdateListener.StartListening();

        RendererAPI::TestInit();
    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE("Renderer Manager Update");
        RendererAPI::TestUpdate();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("Renderer Manager Shutdown");
        TRACE("Shutting down Renderer Manager!")
        m_RenderContext->Shutdown();
        m_UpdateListener.StopListening();
        RendererAPI::TestShutdown();
    }


    RenderingContext& RendererManager::GetContext()
    {
        return *m_RenderContext;
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
