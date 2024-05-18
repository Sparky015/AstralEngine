//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Layer.h"
#include "Ayla/Events/Event.h"

namespace Ayla {


    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void appendLayer(Layer *);
        void insertLayer(Layer *, int);
        void removeLayer(Layer *);

        void appendOverlay(Layer *);
        void insertOverlay(Layer *, int);
        void removeOverlay(Layer *);

        void dispatchEventFromFrontToBack(Event& event);
        void dispatchEventBackToFront(Event& event);

        void update();

        // Disallow copy and move constructors and operator=()
        LayerStack(const LayerStack &) = delete;
        LayerStack(LayerStack &&) = delete;
        LayerStack &operator=(const LayerStack &) = delete;
        LayerStack &operator=(LayerStack &&) = delete;

    private:

        /// Vector holding pointers to the layers
        std::vector<Layer *> m_layers;

        /// Vector holding pointers to the overlay layers
        std::vector<Layer *> m_overlayLayers;
    };

} // Ayla
