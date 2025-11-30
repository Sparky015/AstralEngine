//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"

#include "Core/Engine.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "RHI/RendererAPI.h"

#include "SceneRendererImpl.h"

namespace Astral {

    GraphicsOwnedPtr<SceneRendererImpl> SceneRenderer::m_Renderer{nullptr};


    void SceneRenderer::Init()
    {
        PROFILE_SCOPE("SceneRenderer::Init");

        m_Renderer = CreateGraphicsOwnedPtr<SceneRendererImpl>();
        m_Renderer->Init();
    }


    void SceneRenderer::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown");

        m_Renderer->Shutdown();
        m_Renderer.reset();
    }


    void SceneRenderer::BeginScene(const SceneDescription& sceneDescription)
    {
        m_Renderer->BeginScene(sceneDescription);
    }


    void SceneRenderer::EndScene()
    {
        m_Renderer->EndScene();
    }


    void SceneRenderer::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform)
    {
        m_Renderer->Submit(mesh, material, transform);
    }


    void SceneRenderer::SetRendererSettings(const RendererSettings& rendererSettings)
    {
        m_Renderer->SetRendererSettings(rendererSettings);
    }


    const RendererSettings& SceneRenderer::GetRendererSettings()
    {
        return m_Renderer->GetRendererSettings();
    }


    DescriptorSetHandle SceneRenderer::GetViewportTexture()
    {
        return m_Renderer->GetViewportTexture();
    }


    void SceneRenderer::ResizeViewport(uint32 width, uint32 height)
    {
        m_Renderer->ResizeViewport(width, height);
    }


    RendererDebugStats SceneRenderer::GetRendererDebugStats()
    {
        return RendererAPI::s_RendererCommands->GetRendererDebugStats();
    }

    RendererType SceneRenderer::GetRendererType()
    {
        return m_Renderer->GetType();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::s_RendererCommands->GetAPI();
    }


    void SceneRenderer::SetRendererType(RendererType rendererType)
    {
        RendererSettings rendererSettings = SceneRenderer::GetRendererSettings();
        rendererSettings.RendererType = rendererType;
        SceneRenderer::SetRendererSettings(rendererSettings);
    }

} // Astral