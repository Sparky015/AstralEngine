//
// Created by Andrew Fagan on 1/5/25.
//

#include "WindowComponents.h"

#include "Window/WindowManager.h"
#include "imgui.h"
#include "Debug/Utils/Public/Loggers.h"

namespace Debug {

    void WindowDimensionsComponent()
    {
        Window::WindowManager& windowManager = Window::g_WindowManager;
        ImGui::Text("Window Dimensions: %d px x %d px", windowManager.GetWidth(), windowManager.GetHeight());
    }

    void WindowVsyncStateComponent()
    {
        ImGui::Text("Vsync: %s", Window::g_WindowManager.GetWindow().GetVsyncSetting() == 1 ? "On" : "Off");
    }

    void WindowAPIInfoComponent()
    {
        ImGui::Text("Window API: %s", Window::g_WindowManager.GetWindow().GetVersion().data());
    }

    void VsyncToggleComponent()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
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
        Vec2 framebufferScale = Window::g_WindowManager.GetWindow().GetFramebufferScale();
        ImGui::Text("Window Framebuffer Scale: %.2f x %.2f", framebufferScale.x, framebufferScale.y);
    }

}
