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
        GLFWwindow* getGLFWwindow() override;
    private:
        GLFWwindow* m_window;
        WindowProperties m_windowProperties;

        struct WindowData{
            int width;
            int height;
            std::string title;
            bool vSync; // TODO: Figure out what this is and why I should add it

            EventCallback callback;
        };

        WindowData m_windowData;
    };
}
