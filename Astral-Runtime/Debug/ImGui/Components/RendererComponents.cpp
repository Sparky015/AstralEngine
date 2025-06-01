//
// Created by Andrew Fagan on 1/5/25.
//

#include "RendererComponents.h"

#include <imgui.h>

#include "Core/Engine.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"


namespace Debug {

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
        Astral::RendererContext& rendererContext = Astral::Engine::Get().GetWindowManager().GetWindow().GetRendererContext();
        ImGui::Text("Renderer API: %s", rendererContext.GetRenderingAPI().data());
    }

}
