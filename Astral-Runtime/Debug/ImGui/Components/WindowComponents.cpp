//
// Created by Andrew Fagan on 1/5/25.
//

#include "WindowComponents.h"

#include "Window/WindowManager.h"
#include "imgui.h"
#include "Core/Engine.h"
#include "Debug/Utilities/Loggers.h"

namespace Debug {

    void WindowDimensionsComponent()
    {
        Window::WindowManager& windowManager = Astral::Engine::Get().GetWindowManager();
        ImGui::Text("Window Dimensions: %d px x %d px", windowManager.GetWidth(), windowManager.GetHeight());
    }

    void WindowVsyncStateComponent()
    {
        ImGui::Text("Vsync: %s", Astral::Engine::Get().GetWindowManager().GetWindow().GetVsyncSetting() == 1 ? "On" : "Off");
    }

    void WindowAPIInfoComponent()
    {
        ImGui::Text("Window API: %s", Astral::Engine::Get().GetWindowManager().GetWindow().GetVersion().data());
    }

    void VsyncToggleComponent()
    {
        Window::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        static bool isVsyncEnabled = true;
        if (ImGui::Checkbox("VSync", &isVsyncEnabled))
        {
            if (isVsyncEnabled)
            {
                LOG("Enabled VSync!");
                window.EnableVSync();
            }
            else
            {
                LOG("Disabled VSync!");
                window.DisableVSync();
            }

        };
    }

    void WindowFramebufferScaleComponent()
    {
        Vec2 framebufferScale = Astral::Engine::Get().GetWindowManager().GetWindow().GetFramebufferScale();
        ImGui::Text("Window Framebuffer Scale: %.2f x %.2f", framebufferScale.x, framebufferScale.y);
    }

}
