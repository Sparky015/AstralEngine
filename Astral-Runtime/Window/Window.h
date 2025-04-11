//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include <memory>

#include "Core/CoreMacroDefinitions.h"
#include "Core/Math/Math.h"
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
        virtual void EnableVSync() = 0;
        virtual void DisableVSync() = 0;
        virtual uint8 GetVsyncSetting() = 0;
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual Vec2 GetFramebufferScale() = 0;

        virtual void SetWindowName(std::string_view windowName) = 0;
        virtual void SetWindowDimensions(int width, int height) = 0;

        virtual void* GetNativeWindow() = 0;
        virtual Astral::RendererContext& GetRendererContext() = 0;
        virtual std::string_view GetVersion() = 0;

        static Window* CreateWindow();

    protected:
        static WindowAPI s_WindowAPI;
        std::unique_ptr<Astral::RendererContext> m_RenderContext;

        int m_WindowWidth = 1080; // Default dimensions if user does not set them
        int m_WindowHeight = 720;
        std::string_view m_WindowName = "Astral Engine"; // Default name if name is not set
        uint8 m_Vsync = 1;
    };

} // Window
