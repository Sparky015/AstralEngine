//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include <Ayla/aypch.h>
#include <Ayla/EventSystem/Event.h>

namespace Ayla{

    struct WindowProperties {
        std::string title;
        int width;
        int height;

        WindowProperties(const std::string& newTitle = "Ayla Engine Window", int newWidth = 1280, int newHeight = 720) :
        title(newTitle),
        width(newWidth),
        height(newHeight){}
    };

    class AYLA_API Window {
    public:
        using eventCallback = std::function<void(Event&)>;

        virtual ~Window();

        virtual void onUpdate() = 0;

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual void setEventCallback(const eventCallback& callback) = 0;

        static Window* createWindow(const WindowProperties& = WindowProperties());
    };
}
