//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"

#include "RHI/RendererAPI.h"

namespace Astral {

    GraphicsOwnedPtr<DeferredRenderer> SceneRenderer::m_RendererBackend{nullptr};


    void SceneRenderer::Init()
    {
        m_RendererBackend = CreateGraphicsOwnedPtr<DeferredRenderer>();
        m_RendererBackend->Init();
    }


    void SceneRenderer::Shutdown()
    {
        m_RendererBackend->Shutdown();
        m_RendererBackend.reset();
    }


    void SceneRenderer::BeginScene(const SceneDescription& sceneDescription)
    {
        m_RendererBackend->BeginScene(sceneDescription);
    }


    void SceneRenderer::EndScene()
    {
        m_RendererBackend->EndScene();
    }


    void SceneRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        m_RendererBackend->Submit(mesh, material, transform);
    }


    DescriptorSetHandle SceneRenderer::GetViewportTexture()
    {
        return m_RendererBackend->GetViewportTexture();
    }


    void SceneRenderer::ResizeViewport(uint32 width, uint32 height)
    {
        m_RendererBackend->ResizeViewport(width, height);
    }


    RendererDebugStats SceneRenderer::GetRendererDebugStats()
    {
        return RendererAPI::s_RendererCommands->GetNumberOfDrawCalls();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::s_RendererCommands->GetAPI();
    }


} // Astral