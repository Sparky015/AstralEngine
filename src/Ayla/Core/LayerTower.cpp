//
// Created by Andrew Fagan on 4/24/24.
//

#include "LayerTower.h"

namespace Ayla {

    LayerTower::LayerTower() {

    }

    LayerTower::~LayerTower() {

    }

    LayerTower* LayerTower::m_instance = nullptr;
    LayerTower& LayerTower::get() {
        if (m_instance == nullptr){
            m_instance = new LayerTower();
        }
        return *m_instance;
    }

    void LayerTower::appendLayer(Ayla::Layer* layer) {
        m_layers.push_back(layer);
    }

    void LayerTower::insertLayer(Ayla::Layer* layer, int index) {

    }

    void LayerTower::removeLayer(Ayla::Layer* layer) {

    }

    void LayerTower::appendOverlayLayer(Ayla::Layer* layer) {
        m_overlayLayers.push_back(layer);
    }

    void LayerTower::insertOverlayLayer(Ayla::Layer* layer, int index) {

    }

    void LayerTower::removeOverlayLayer(Ayla::Layer* layer) {

    }

    void LayerTower::dispatchEvent(Ayla::Event& event) {
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            layer->callbackFn(event);
        }
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}

        }
    }


} // Ayla