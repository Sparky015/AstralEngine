//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Layer.h"
#include "Ayla/Events/Event.h"

namespace Ayla::Core::Layers
{


    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void appendLayer(ILayer* layer);
        void insertLayer(ILayer*, int position);
        void removeLayer(ILayer* layer);

        void appendOverlay(ILayer* layer);
        void insertOverlay(ILayer* layer, int position);
        void removeOverlay(ILayer* layer);

        void dispatchEventFromFrontToBack(Event& event);
        void dispatchEventBackToFront(Event& event);

        void update();
        void renderImGui();

        // Disallow copy and move constructors and operator=()
        LayerStack(const LayerStack &) = delete;
        LayerStack(LayerStack &&) = delete;
        LayerStack &operator=(const LayerStack &) = delete;
        LayerStack &operator=(LayerStack &&) = delete;

    private:

        /// Vector holding pointers to the layers
        std::vector<ILayer *> m_layers;

        /// Vector holding pointers to the overlay layers
        std::vector<ILayer *> m_overlayLayers;
    };

} // Ayla
