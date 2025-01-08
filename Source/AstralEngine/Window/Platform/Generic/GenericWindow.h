//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Window/Window.h"

#include "pch.h"

#include "GLFW/glfw3.h"


namespace Window {

    class GenericWindow : public Window
    {
    public:
        GenericWindow();

        void Init() override;
        void Update() override;
        void Shutdown() override;
        void SwapBuffers() override;
        std::string_view GetVersion() override;
        void EnableVSync() override;
        void DisableVSync() override;
        uint8 GetVsyncSetting() override { return m_Vsync; }
        int GetWidth() override;
        int GetHeight() override;
        Vec2 GetFramebufferScale() override;

        Graphics::RendererContext & GetRendererContext() override;
        void* GetNativeWindow() override;

    private:
        void SetGLFWCallbacks();

        static bool m_IsGLFWInitialized;
        GLFWwindow* m_Window;
        int m_WindowWidth = 800;
        int m_WindowHeight = 800;
        uint8 m_Vsync = 1;
    };

} // Window
