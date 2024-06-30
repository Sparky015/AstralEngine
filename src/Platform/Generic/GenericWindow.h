//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/Window/Window.h"
#include "GLFW/glfw3.h"

namespace Ayla::Windows{

    static bool s_isGLFWInitialized = false;
    static void GLFWErrorCallback(int error, const char* description);

    class GenericWindow : public Window {
    public:

        explicit GenericWindow(const WindowProperties &properties = WindowProperties());
        ~GenericWindow() override;

        void init(const WindowProperties &properties);
        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;
        [[nodiscard]] float getDisplayFramebufferScaleX() const override;
        [[nodiscard]] float getDisplayFramebufferScaleY() const override;

        void setEventCallback(const EventCallback &callback) override;
        void update() override;

    private:
        GLFWwindow* m_window;
        WindowProperties m_windowProperties;

        struct WindowData {
            int width;
            int height;
            float displayFramebufferScaleX;
            float displayFramebufferScaleY;
            std::string title;
            bool vSync;

            EventCallback callback;
        };

        WindowData m_windowData;


    };
}
