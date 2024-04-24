//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include <Ayla/Core/Layer.h>
#include <Ayla/EventSystem/Event.h>

namespace Ayla {

    class LayerTower {
    public:
        static LayerTower& get();

        void appendLayer(Layer*);
        void insertLayer(Layer*, int);
        void removeLayer(Layer*);

        void appendOverlayLayer(Layer*);
        void insertOverlayLayer(Layer*, int);
        void removeOverlayLayer(Layer*);

        void dispatchEvent(Event&);


        LayerTower(const LayerTower&) = delete;
        LayerTower(LayerTower&&) = delete;
        LayerTower& operator= (const LayerTower&) = delete;
        LayerTower& operator= (LayerTower&&) = delete;

    private:
        LayerTower();
        ~LayerTower();

        // Systems where the layer
        static LayerTower* m_instance;
        std::vector<Layer*> m_layers;
        std::vector<Layer*> m_overlayLayers;
    };

} // Ayla
