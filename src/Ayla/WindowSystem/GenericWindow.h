//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once

#include "Ayla/WindowSystem/Window.h"
#include "GLFW/glfw3.h"

namespace Ayla{

    static bool s_isGLFWInitialized = false;

    /**
 * @class GenericWindow
 * @brief This class represents a generic window in the Ayla system.
 * @details This class manages the window properties and supports several operations such as
 * initialization, getting the width and height, setting event callbacks, etc.
 * It also includes an internal struct for managing window data, including width, height, title and other attributes.
 * The class extends from the Window base class.
 */
    class GenericWindow : public Window {
    public:
        /**
         * @brief Constructor for GenericWindow, uses WindowProperties as its argument.
         * @param properties The properties to use when creating the window.
         */
        GenericWindow(const WindowProperties &properties = WindowProperties());

        /**
         * @brief Destructor for the GenericWindow.
         */
        ~GenericWindow() override;

        /**
         * @brief Initializes the GenericWindow using the provided properties.
         * @param properties The properties to use when initializing the window.
         */
        void init(const WindowProperties &properties);

        /**
         * @brief Getter for the window's width.
         * @return Width of the window.
         */
        int getWidth() const override;

        /**
         * @brief Getter for the window's height.
         * @return Height of the window.
         */
        int getHeight() const override;

        /**
         * @brief Sets the EventCallback for the window.
         * @param callback The callback function to use for window events.
         */
        void setEventCallback(const EventCallback &callback) override;

        /**
         * @brief This method is called to update the window.
         */
        void onUpdate() override;


    private:
        GLFWwindow *m_window;  ///< Pointer to the GLFW Window object.
        WindowProperties m_windowProperties; /**< WindowProperties structure instance holding key properties of the window. */

        /**
         * @struct WindowData
         * @brief Structure to hold window data.
         * @details This struct includes attributes such as width, height, title, vsync state and event callback.
         */
        struct WindowData {
            int width;  ///< Width of the window.
            int height;  ///< Height of the window.
            std::string title;  ///< Title of the window.
            bool vSync;  ///< Represents whether VSync is enabled or not.

            EventCallback callback;  ///< Event callback function.
        };

        WindowData m_windowData;  ///< Instance of WindowData struct that holds key information about the window.
    };
}
