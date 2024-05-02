//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Ayla/EventSystem/Event.h"

namespace Ayla {

    /**
 * @class Layer
 * @brief Defines a layer object with event handling facilities */
    class Layer {
    public:
        /**
         * @brief Default constructor for the Layer class.
         */
        Layer();

        /**
         * @brief Destructor for the Layer class.
         */
        virtual ~Layer();

        /**
         * @brief Attaches a layer to the layer tower.
         */
        void attachLayer();

        /**
         * @brief Attaches a overlay to the layer tower.
         */
        void attachOverlay();

        /**
         * @brief Allows for functionality when a layer is attached
         */
        virtual void onAttach() = 0;

        /**
         * @brief Detaches a layer from the layer tower.
         */
        void detachLayer();

        /**
        * @brief Detaches an overlay from the layer tower.
        */
        void detachOverlay();

        /**
         * @brief Allows for functionality when a layer is detached.
         */
        virtual void onDetach() = 0;

        /**
         * @brief Allows for functionality to be done on every update of the application
         */
         virtual void onUpdate() = 0;

        /**
        * @brief Sets a callback function to handle events.
        * @param callback The function to call when an event occurs.
        */
        void setCallback(std::function<void(Event&)> callback);

        /**
         * @brief Dispatches an event to the callback function.
         * @param event The event to dispatch.
         */
        virtual void onEvent(Event &event) = 0;

        /**
         * @brief Checks if the layer is enabled.
         * @return true if enabled, false otherwise.
         */
        bool isEnabled();

        /**
         * @brief Enables the layer.
         */
        void enable();

        /**
         * @brief Disables the layer.
         */
        void disable();

    private:

        bool m_isEnabled; ///< Represents if the layer is enabled or not
        bool m_isInitializedInTower; ///< Represents if the layer is initialized in the tower or not
        std::function<void(Event & )> m_callback; ///< Function to call when an event occurs
    };



} // Ayla


