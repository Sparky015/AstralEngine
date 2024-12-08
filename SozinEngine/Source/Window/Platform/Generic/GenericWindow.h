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
        std::string GetVersion() override;
        void EnableVsync() override;
        void DisableVsync() override;
        int GetWidth() override;
        int GetHeight() override;

        Renderer::RendererContext & GetRendererContext() override;
        void* GetNativeWindow() override;

    private:
        void SetGLFWCallbacks();

        static bool m_IsGLFWInitialized;
        GLFWwindow* m_Window;
        int m_WindowWidth = 1024;
        int m_WindowHeight = 768;
    };

} // Window
