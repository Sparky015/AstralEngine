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

    void LayerTower::appendLayer(Layer* layer) {
        m_layers.push_back(layer);
    }

    void LayerTower::insertLayer(Layer* layer, int index) {

    }

    void LayerTower::removeLayer(Layer* layer) {

    }

    void LayerTower::appendOverlay(Layer* layer) {
        m_overlayLayers.push_back(layer);
    }

    void LayerTower::insertOverlay(Layer* layer, int index) {

    }

    void LayerTower::removeOverlay(Layer* layer) {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }

    void LayerTower::sendEventDownTower(Event& event) {
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
    }

    void LayerTower::sendEventUpTower(Event& event) {
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }

    }



} // Ayla