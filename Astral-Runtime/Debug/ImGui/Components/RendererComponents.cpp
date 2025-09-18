//
// Created by Andrew Fagan on 1/5/25.
//

#include "RendererComponents.h"

#include <imgui.h>

#include "Core/Engine.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"


namespace Astral {

    void FPSComponent()
    {
        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Framerate: %.1f", io.Framerate);
    }

    void FrameTimeComponent()
    {
        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
    }

    void DrawCallsPerFrameComponent()
    {
        ImGui::Text("Draw Calls/Frame: %d", Astral::SceneRenderer::GetRendererDebugStats().NumberOfDrawCalls);
        ImGui::Text("Triangles/Frame: %d", Astral::SceneRenderer::GetRendererDebugStats().NumberOfTriangles);
    }

    void RendererAPIComponent()
    {
        Astral::RenderingContext& rendererContext = Astral::Engine::Get().GetRendererManager().GetContext();
        ImGui::Text("Renderer API: %s", rendererContext.GetRenderingAPI().data());
    }


    void RendererAPIValidationStatus()
    {
#ifdef ASTRAL_DISABLE_RENDERING_API_VALIDATION
        ImGui::Text("Renderer API Validation: Off");
#else
        ImGui::Text("Renderer API Validation: On");
#endif
    }


    void RendererViewportSizeComponent()
    {
        UVec2 viewportSize = SceneRenderer::GetViewportSize();
        ImGui::Text("Viewport Dimensions: %d px x %d px", viewportSize.x, viewportSize.y);
    }


    void RendererTypeSelector()
    {
        ImGui::Text("Renderer Type:");
        ImGui::SameLine();

        if (ImGui::BeginCombo("##DebugMenuRendererTypeSelector", RendererTypeToString(SceneRenderer::GetRendererType()).data()))
        {

            if (ImGui::Selectable("Deferred"))
            {
                SceneRenderer::SetRendererType(RendererType::DEFERRED);
            }

            if (ImGui::Selectable("Forward"))
            {
                SceneRenderer::SetRendererType(RendererType::FORWARD);
            }

            ImGui::EndCombo();
        }
    }


    void VsyncToggleComponent()
    {
        RendererSettings rendererSettings = SceneRenderer::GetRendererSettings();

        ImGui::Text("VSync: ");
        ImGui::SameLine();
        ImGui::Checkbox("##VSyncToggle", &rendererSettings.IsVSyncEnabled);

        SceneRenderer::SetRendererSettings(rendererSettings);
    }


    void FrustumCullingToggleComponent()
    {
        RendererSettings rendererSettings = SceneRenderer::GetRendererSettings();

        ImGui::Text("Frustum Culling: ");
        ImGui::SameLine();
        ImGui::Checkbox("##FrustumCullingToggle", &rendererSettings.IsFrustumCullingEnabled);

        SceneRenderer::SetRendererSettings(rendererSettings);
    }


    void RendererDebugViewComponent()
    {
        RendererSettings rendererSettings = SceneRenderer::GetRendererSettings();

        ImGui::Text("Debug View: ");
        ImGui::SetNextItemWidth(-1);
        ImGui::SameLine();

        if (ImGui::BeginCombo("##DebugMenuRendererDebugViewSelector", RendererDebugViewToString(rendererSettings.DebugView).data()))
        {

            if (ImGui::Selectable("None"))
            {
                rendererSettings.DebugView = RendererDebugView::NONE;
            }
            if (ImGui::Selectable("GBuffer-Albedo"))
            {
                rendererSettings.DebugView = RendererDebugView::GBUFFER_ALBEDO;
            }
            if (ImGui::Selectable("GBuffer-Roughness"))
            {
                rendererSettings.DebugView = RendererDebugView::GBUFFER_ROUGHNESS;
            }
            if (ImGui::Selectable("GBuffer-Metallic"))
            {
                rendererSettings.DebugView = RendererDebugView::GBUFFER_METALLIC;
            }
            if (ImGui::Selectable("GBuffer-Emission"))
            {
                rendererSettings.DebugView = RendererDebugView::GBUFFER_EMISSION;
            }
            if (ImGui::Selectable("GBuffer-Normal"))
            {
                rendererSettings.DebugView = RendererDebugView::GBUFFER_NORMAL;
            }
            // if (ImGui::Selectable("Depth")) 
            // {
            //     rendererSettings.DebugView = RendererDebugView::DEPTH;
            // }
            if (ImGui::Selectable("Cascaded Shadow Map Boundaries"))
            {
                rendererSettings.DebugView = RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES;
            }

            ImGui::EndCombo();
        }


        ImGui::Text("Shadows: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::Checkbox("##ShadowsToggle", &rendererSettings.IsShadowsOn);

        ImGui::Text("Shadow Cascades: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputInt("##ShadowCascadesInput", &rendererSettings.NumShadowCascades);

        int shadowMapResolution = rendererSettings.ShadowMapResolution;
        ImGui::Text("Shadow Map Resolution: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputInt("##ShadowMapResolutionInput", &shadowMapResolution);

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            rendererSettings.ShadowMapResolution = shadowMapResolution;
        }

        ImGui::Text("Shadow Map Bias: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##ShadowMapBiasInput", &rendererSettings.ShadowMapBias, 0, 0, "%.5f");

        SceneRenderer::SetRendererSettings(rendererSettings);
    }

}
