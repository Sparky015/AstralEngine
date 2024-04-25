//
// Created by sparky on 4/16/2024.
//
#pragma once
#include "Ayla/Core/Core.h"
#include "Ayla/WindowSystem/Window.h"


namespace Ayla {

    /**
* @class Application
* @brief A class encapsulating the application details for this project.
*
* This class manages the execution and the runtime responses of the Ayla project.
*/
    class AYLA_API Application{
    public:
            /**
             * @brief Default Constructor for Application.
             * Constructs a new Application instance.
             */
            Application();

            /**
             * @brief Destructor for Application.
             * Cleans up the Application instance.
             */
             ~Application();

            /**
             * @brief Starts and runs the Application.
             * Initiates and maintains the Application runtime loop.
             */
             void Run();

            /**
             * @brief Event Handler for the Application.
             * Specifies what actions to take whenever an event is triggered.
             * @param [in] e The event that was triggered.
             */
             void onEvent(Event&);

    private:

        bool m_appIsRunning = true;

        /**
         * @brief Pointer for managing a Window instance.
         * This instance handles the application window management & display.
         */
        std::unique_ptr<Window> m_window;
    };

/**
 * @brief Creates a new Application instance.
 * This function is defined client side and contains the details for setting up a new Application.
 * @return An Application pointer pointing to the new Application instance.
 */
    AYLA_API extern Application*

    CreateApplication();

} // Ayla


