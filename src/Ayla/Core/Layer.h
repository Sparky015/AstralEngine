//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include <Ayla/EventSystem/Event.h>

namespace Ayla {

    /**
 * @class Layer
 * @brief Defines a layer object with event handling facilities
 */
    class Layer {
    public:
        /**
         * @brief Default constructor for the Layer class.
         */
        Layer();

        /**
         * @brief Destructor for the Layer class.
         */
        ~Layer();

        /**
         * @brief Attaches a layer.
         */
        void attachLayer();

        /**
         * @brief Detaches a layer.
         */
        void detachLayer();

        /**
         * @brief Removes a layer.
         */
        void removeLayer();

        /**
         * @brief Sets a callback function to handle events.
         *
         * @param callback The function to call when an event occurs.
         */
        void setCallback(std::function<void(Event & )> callback);

        /**
         * @brief Dispatches an event to the callback function.
         *
         * @param event The event to dispatch.
         */
        void onEvent(Event &event);

        /**
         * @brief Checks if the layer is enabled.
         *
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

/**
 * @class ExampleLayer
 * @brief An example subclass of Layer, which provides additional layer information.
 *
 * @invariant The event category flag will be set to (InputCategory | ApplicationCategory) on construction.
 */
    class ExampleLayer : public Layer {

    public:

        /**
         * @brief Retrieves the type of the layer.
         *
         * @return Integer representing the type of the layer.
         */
        int getLayerType();

        /**
         * @brief Retrieves the event category flags for this layer.
         *
         * @return Integer representing the event category flags.
         */
        int getEventCategoryFlags();

    private:
        int m_eventCategory = (InputCategory |
                               ApplicationCategory); ///< Bitmask Events category {InputCategory | ApplicationCategory}
    };


} // Ayla


