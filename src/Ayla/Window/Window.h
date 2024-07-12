//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/Events/Event.h"
#include "Ayla/aypch.h"


namespace Ayla::Windows { // maybe change namespace to Display or Viewport instead of Windows?
    using EventCallback = std::function<void(Events::IEvent& event)>;

    /** Values used to create the desire window */
    struct WindowProperties
    {
        std::string Title;
        int Width;
        int Height;

        WindowProperties(std::string title, int width, int height) :
                Title(std::move(title)),
                Width(width),
                Height(height)
        {}
    };


    class Window
    {
    public:
        Window() = default;

        virtual ~Window() = default;


        /** Updates the window by swapping the buffers. */
        virtual void Update() = 0;

        /** Retrieves the width of the window. */
        [[nodiscard]] virtual int GetWidth() const = 0;

        /** Retrieves the height of the window. */
        [[nodiscard]] virtual int GetHeight() const = 0;

        /**  */
        [[nodiscard]] virtual float GetDisplayFramebufferScaleX() const = 0;

        /**  */
        [[nodiscard]] virtual float GetDisplayFramebufferScaleY() const = 0;

        /**  */
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        /**  */
        virtual void SetEventCallback(const EventCallback& callback) = 0;

        /**  */
        static Window* CreateWindow(const WindowProperties& properties);
    };

} // namespace Ayla::Windows
