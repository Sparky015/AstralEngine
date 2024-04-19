//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/aypch.h"
#include "Ayla/EventSystem/Event.h"
#include "GLFW/glfw3.h"

namespace Ayla{

    struct WindowProperties {
        std::string title;
        int width;
        int height;

        WindowProperties(std::string title = "Ayla Engine Window", int width = 1280, int height = 720) :
        title(std::move(title)),
        width(width),
        height(height){}
    };

    class AYLA_API Window {
    public:
        using EventCallback = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;

        virtual void setEventCallback(const EventCallback& callback) = 0;

        virtual GLFWwindow* getGLFWwindow() = 0;

        static Window* createWindow(const WindowProperties& properties = WindowProperties());
    };
}
