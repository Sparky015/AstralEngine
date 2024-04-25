//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/aypch.h"
#include "Ayla/EventSystem/Event.h"
#include "GLFW/glfw3.h"

namespace Ayla{

    /**
 * @brief Window Properties Struct.
 *
 * Defines the basic properties of a Window.
*/
    struct WindowProperties {
        std::string title; /**< The title of the window */
        int width; /**< The width of the window in pixels */
        int height; /**< The height of the window in pixels */

        /**
         * @brief Constructor for WindowProperties
         *
         * @param title window title
         * @param width window width in pixels
         * @param height window height in pixels
         */
        WindowProperties(std::string title = "Ayla Engine Window", int width = 1280, int height = 720) :
                title(std::move(title)),
                width(width),
                height(height) {}
    };

/**
 * @brief Window Class.
 *
 * Defines the basic set of operations required by a Window.
*/
    class AYLA_API Window{
            public:
            /**
             * @brief EventCallback type definition
             * An std::function type to handle window events.
            */
            using EventCallback = std::function<void(Event&)>;

            virtual ~Window() = default; /**< Destructor. */

            /**
             * @brief Updates the window state.
             * Must be implemented by derived classes.
            */
            virtual void onUpdate() = 0;

            /**
             * @brief Returns the window's width
             *
             * @return int window's width in pixels
            */
            [[nodiscard]] virtual int getWidth() const = 0;

            /**
             * @brief Returns the window's height
             *
             * @return int window's height in pixels
            */
            [[nodiscard]] virtual int getHeight() const = 0;

            /**
             * @brief Sets the event callback function for the window
             *
             * @param callback EventCallback function
            */
            virtual void setEventCallback(const EventCallback& callback) = 0;

            /**
             * @brief Static helper function to create a window
             *
             * @param properties window's properties, defaults to WindowProperties()
             * @return Window* pointer to a Window object
             */
            static Window* createWindow(const WindowProperties& properties = WindowProperties());
    };
}
