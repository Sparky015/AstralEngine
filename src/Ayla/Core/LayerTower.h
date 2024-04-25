//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include <Ayla/Core/Layer.h>
#include <Ayla/EventSystem/Event.h>

namespace Ayla {

    /**
 * @class LayerTower
 *
 * @brief Class responsible for managing layering system and event propagation
 *
 */
    class LayerTower {
    public:
        /**
         * @brief Singleton instance accessor
         *
         * @return LayerTower& - reference to the singleton instance
         *
         */
        static LayerTower &get();

        /**
         * @brief Append a layer to the layer list
         *
         * @param[in] Layer* - Pointer to a Layer object
         *
         */
        void appendLayer(Layer *);

        /**
         * @brief Insert a layer at a particular index in the layer list
         *
         * @param[in] Layer* - Pointer to a Layer object
         * @param[in] int - The index where the layer should be inserted
         *
         */
        void insertLayer(Layer *, int);

        /**
         * @brief Remove a Layer from the layer list
         *
         * @param[in] Layer* - Pointer to a Layer to be removed
         *
         */
        void removeLayer(Layer *);

        /**
         * @brief Append an overlay layer to the overlay layer list
         *
         * @param[in] Layer* - Pointer to an overlay Layer
         *
         */
        void appendOverlayLayer(Layer *);

        /**
         * @brief Insert an overlay layer at a particular index in the overlay layer list
         *
         * @param[in] Layer* - Pointer to an overlay Layer
         * @param[in] int - The index where the overlay layer should be inserted
         */
        void insertOverlayLayer(Layer *, int);

        /**
         * @brief Remove an overlay layer from the overlay layer list
         *
         * @param[in] Layer* - Pointer to an overlay Layer to be removed
         *
         */
        void removeOverlayLayer(Layer *);

        /**
         * @brief Propagate an event up the Layer Tower
         *
         * @param[in] Event& - Reference to an Event
         *
         */
        void sendEventUpTower(Event &);

        /**
         * @brief Propagate an event down the Layer Tower
         *
         * @param[in] Event& - Reference to an Event
         *
         */
        void sendEventDownTower(Event &);

        // Disallow copy and move constructors and operator=()
        LayerTower(const LayerTower &) = delete;

        LayerTower(LayerTower &&) = delete;

        LayerTower &operator=(const LayerTower &) = delete;

        LayerTower &operator=(LayerTower &&) = delete;

    private:
        /// Private constructor for singleton pattern
        LayerTower();

        /// Private destructor for singleton pattern
        ~LayerTower();

        /// Singleton instance
        static LayerTower *m_instance;

        /// Vector holding pointers to the layers
        std::vector<Layer *> m_layers;

        /// Vector holding pointers to the overlay layers
        std::vector<Layer *> m_overlayLayers;
    };

} // Ayla
