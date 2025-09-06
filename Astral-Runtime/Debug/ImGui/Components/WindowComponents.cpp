//
// Created by Andrew Fagan on 1/5/25.
//

#include "WindowComponents.h"

#include "Window/WindowManager.h"
#include "imgui.h"
#include "Core/Engine.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral {

    void WindowDimensionsComponent()
    {
        Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        ImGui::Text("Window Dimensions: %d px x %d px", window.GetWidth(), window.GetHeight());
    }

    void WindowAPIInfoComponent()
    {
        ImGui::Text("Window API: %s", Astral::Engine::Get().GetWindowManager().GetWindow().GetVersion().data());
    }

    void WindowFramebufferScaleComponent()
    {
        Vec2 framebufferScale = Astral::Engine::Get().GetWindowManager().GetWindow().GetFramebufferScale();
        ImGui::Text("Window Framebuffer Scale: %.2f x %.2f", framebufferScale.x, framebufferScale.y);
    }

}
