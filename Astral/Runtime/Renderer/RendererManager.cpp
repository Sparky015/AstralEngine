//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Asset/AssetManager.h"
#include "Common/CubeLUT.h"
#include "Core/Engine.h"
#include "Profiler/Instrumentation/ScopeProfiler.h"
#include "Core/Utilities/Loggers.h"
#include "RHI/RendererAPI.h"
#include "Scenes/Scene.h"

namespace Astral {

    RendererManager::RendererManager()
    {
        AE_TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        AE_TRACE("Destroying Renderer Manager!")
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE("RendererManager::Init");
        AE_TRACE("Initializing Renderer Manager!");
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();

        SceneRenderer::Init();
    }


    void RendererManager::Update()
    {
        SceneRenderer::RenderScene();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("RendererManager::Shutdown");
        AE_TRACE("Shutting down Renderer Manager!")

        SceneRenderer::Shutdown();
        m_RenderContext->Shutdown();
    }


    RenderingContext& RendererManager::GetContext()
    {
        return *m_RenderContext;
    }

}
