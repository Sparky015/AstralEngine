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
        ImGui::Text("Draw Calls/Frame: %d", Astral::SceneRenderer::GetDrawCallsPerFrame());
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

}
