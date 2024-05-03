//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Ayla/Core/Layer.h"
#include "Ayla/EventSystem/Event.h"

namespace Ayla {


    class LayerTower {
    public:


        static LayerTower& get();
        void appendLayer(Layer *);
        void insertLayer(Layer *, int);
        void removeLayer(Layer *);
        void appendOverlay(Layer *);
        void insertOverlay(Layer *, int);
        void removeOverlay(Layer *);
        void sendEventUpTower(Event &);
        void sendEventDownTower(Event &);

        void onUpdate();

        // Disallow copy and move constructors and operator=()
        LayerTower(const LayerTower &) = delete;
        LayerTower(LayerTower &&) = delete;
        LayerTower &operator=(const LayerTower &) = delete;
        LayerTower &operator=(LayerTower &&) = delete;

    private:

        LayerTower();


        ~LayerTower();

        static LayerTower *m_instance;

        /// Vector holding pointers to the layers
        std::vector<Layer *> m_layers;

        /// Vector holding pointers to the overlay layers
        std::vector<Layer *> m_overlayLayers;
    };

} // Ayla
