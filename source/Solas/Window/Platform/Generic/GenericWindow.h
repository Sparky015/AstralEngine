//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Solas/Events/Event.h"
#include "Solas/Renderer/OpenGLRenderingContext.h"
#include "Solas/Window/Window.h"
#include "Solas/aypch.h"

#include "GLFW/glfw3.h"

namespace Solas::Windows{

    /** Window class that is designed for cross-platform compatibility. A generic window version that any major OS can use (using GLFW). */
    class GenericWindow : public Window {
    public:

        explicit GenericWindow(const WindowProperties& properties);
        ~GenericWindow() override;

        /** Initializes the GLFW and the window context and sets the window callbacks. */
        void Init(const WindowProperties &windowProperties);

        /** Returns the pixel width of the window */
        [[nodiscard]] int GetWidth() const override;

        /** Returns the pixel height of the window */
        [[nodiscard]] int GetHeight() const override;

        /** Returns the framebuffer to pixel ratio in the X dimension. */
        [[nodiscard]] float GetDisplayFramebufferScaleX() const override;

        /** Returns the framebuffer to pixel ratio in the Y dimension. */
        [[nodiscard]] float GetDisplayFramebufferScaleY() const override;

        /** Returns a pointer to the native window class or struct being used. (In this case GLFWwindow) */
        [[nodiscard]] void* GetNativeWindow() const override;

        /** Defines a callback that is called when events are raised by the GLFW window. */
        virtual void SetEventCallback(const EventCallback& callback) override;

        /** Polls the window events and swaps the buffer. */
        void Update() override;

        /** Enables VSync for the window. */
        void EnableVSync() override;

        /** Disables VSync for the window. */
        void DisableVSync() override;

    private:
        GLFWwindow* m_Window;
        WindowProperties m_WindowProperties;

        struct WindowData {
            int Width;
            int Height;
            float DisplayFramebufferScaleX;
            float DisplayFramebufferScaleY;
            std::string Title;
            bool IsVSyncEnabled;

            EventCallback Callback;
        };

        WindowData m_WindowData;
        static bool m_IsGlfwInitialized;

        Renderer::OpenGLRenderingContext m_Context;

    };

    static void GlfwErrorCallback(int error, const char* description);

} // namespace Solas::Windows
