//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/aypch.h"
#include "Ayla/Events/Event.h"


using namespace Ayla::Events;

namespace Ayla::Windows{ // maybe change namespace to Display or Viewport instead of Windows?


    // Values used to create the desire window
    struct WindowProperties {
        std::string title;
        float width;
        float height;

        WindowProperties(std::string title = "Ayla ClientSetup Window", float width = 1024.0f, float height = 768.0f) :
                title(std::move(title)),
                width(width),
                height(height) {}
    };



    class AYLA_API Window{
    public:
            using EventCallback = std::function<void(Event&)>;

            virtual ~Window() = default;
            virtual void update() = 0;

            [[nodiscard]] virtual int getWidth() const = 0;
            [[nodiscard]] virtual int getHeight() const = 0;
            [[nodiscard]] virtual float getDisplayFramebufferScaleX() const = 0;
            [[nodiscard]] virtual float getDisplayFramebufferScaleY() const = 0;
            [[nodiscard]] virtual void* getNativeWindow() const = 0;

            virtual void setEventCallback(const EventCallback& callback) = 0;
            static Window* createWindow(const WindowProperties& properties = WindowProperties());
    };

}
