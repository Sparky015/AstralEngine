//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/RendererContext.h"

namespace Window {

    enum class WindowAPI
    {
        GLFW = 0, Cocoa, Win32
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
        virtual void SwapBuffers() = 0;
        virtual void EnableVsync() = 0;
        virtual void DisableVsync() = 0;

        virtual void* GetNativeWindow() = 0;
        virtual Renderer::RendererContext& GetRendererContext() = 0;
        virtual std::string GetVersion() = 0;

        static Window* CreateWindow();

    protected:
        static WindowAPI s_RendererAPI;
        std::unique_ptr<Renderer::RendererContext> m_RenderContext;
    };

} // Window
