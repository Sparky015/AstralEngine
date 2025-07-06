//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"

#include "Core/Engine.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "RHI/RendererAPI.h"

namespace Astral {

    GraphicsOwnedPtr<Renderer> SceneRenderer::m_Renderer{nullptr};
    bool SceneRenderer::m_IsRendererTypeUpdateNeeded{};
    RendererType SceneRenderer::m_NewRendererType{};


    void SceneRenderer::Init()
    {
        m_Renderer = CreateGraphicsOwnedPtr<DeferredRenderer>();
        m_Renderer->Init();
    }


    void SceneRenderer::Shutdown()
    {
        m_Renderer->Shutdown();
        m_Renderer.reset();
    }


    void SceneRenderer::BeginScene(const SceneDescription& sceneDescription)
    {
        if (m_IsRendererTypeUpdateNeeded) { UpdateRendererType(); }
        m_Renderer->BeginScene(sceneDescription);
    }


    void SceneRenderer::EndScene()
    {
        m_Renderer->EndScene();
    }


    void SceneRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        m_Renderer->Submit(mesh, material, transform);
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
        return RendererAPI::s_RendererCommands->GetNumberOfDrawCalls();
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
        if (m_Renderer->GetType() == rendererType) { return; }

        m_NewRendererType = rendererType;
        m_IsRendererTypeUpdateNeeded = true;
    }


    void SceneRenderer::UpdateRendererType()
    {
        WARN("Changing renderer type!")

        m_Renderer->Shutdown();
        m_Renderer.reset();

        Engine::Get().GetImGuiManager().Shutdown();

        switch (m_NewRendererType)
        {
           case RendererType::DEFERRED: m_Renderer = CreateGraphicsOwnedPtr<DeferredRenderer>(); break;
           case RendererType::FORWARD: m_Renderer = CreateGraphicsOwnedPtr<ForwardRenderer>(); break;
        }

        Engine::Get().GetImGuiManager().Init();

        m_Renderer->Init();

        m_IsRendererTypeUpdateNeeded = false;
    }

} // Astral