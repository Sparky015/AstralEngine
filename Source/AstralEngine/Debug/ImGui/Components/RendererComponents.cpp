//
// Created by Andrew Fagan on 1/5/25.
//

#include "RendererComponents.h"

#include <imgui.h>

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommands.h"
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

    void DrawCallsPerFrame()
    {
        ImGui::Text("Draw Calls/Frame: %d", Graphics::RendererCommand::GetNumberOfDrawCallsPerFrame());
    }

    void RendererAPIComponent()
    {
        Graphics::RendererContext& rendererContext = Window::g_WindowManager.GetWindow().GetRendererContext();
        ImGui::Text("Renderer API: %s", rendererContext.GetRenderingAPI().c_str());
    }

    void SystemGPUInfo()
    {
        Graphics::RendererContext& rendererContext = Window::g_WindowManager.GetWindow().GetRendererContext();
        ImGui::Text("GPU Vendor: %s", rendererContext.GetGPUVendor().c_str());
        ImGui::Text("Graphics Processor: %s", rendererContext.GetGraphicsProcessor().c_str());
    }
}
