//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include <Ayla/WindowSystem/Window.h>
#include <GLFW/glfw3.h>

namespace Ayla{

    static bool s_isGLFWInitialized = false;

    class GenericWindow : public Window {
    public:
        GenericWindow(const WindowProperties &properties = WindowProperties());
        ~GenericWindow() override;

        void init(const WindowProperties& properties);
        int getWidth() const override;
        int getHeight() const override;
        void setEventCallback(const EventCallback& callback) override;
        void onUpdate() override;
        GLFWwindow* getGLFWwindow();
    private:
        GLFWwindow* m_window;
        WindowProperties m_windowProperties;
        EventCallback m_callback;
    };
}
