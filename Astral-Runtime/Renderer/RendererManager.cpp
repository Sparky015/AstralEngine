//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "RHI/RendererAPI.h"

namespace Astral {

    RendererManager::RendererManager()
    {
        TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        TRACE("Destroying Renderer Manager!")
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE("Renderer Manager Initialization");
        TRACE("Initializing Renderer Manager!");
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();

        SceneRenderer::Init();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("Renderer Manager Shutdown");
        TRACE("Shutting down Renderer Manager!")

        SceneRenderer::Shutdown();
        m_RenderContext->Shutdown();
    }


    RenderingContext& RendererManager::GetContext()
    {
        return *m_RenderContext;
    }

}
