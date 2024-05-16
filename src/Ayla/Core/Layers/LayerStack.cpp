//
// Created by Andrew Fagan on 4/24/24.
//

#include "LayerStack.h"

namespace Ayla {

    LayerStack::LayerStack() {

    }

    LayerStack::~LayerStack() {

    }

    void LayerStack::appendLayer(Layer* layer) {
        m_layers.push_back(layer);
    }

    void LayerStack::insertLayer(Layer* layer, int index) {

    }

    void LayerStack::removeLayer(Layer* layer) {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }

    void LayerStack::appendOverlay(Layer* layer) {
        m_overlayLayers.push_back(layer);
    }

    void LayerStack::insertOverlay(Layer* layer, int index) {

    }

    void LayerStack::removeOverlay(Layer* layer) {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }

    void LayerStack::sendEventDownTower(Event& event) {
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
    }

    void LayerStack::sendEventUpTower(Event& event) {
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            layer->onEvent(event);
        }

    }

    void LayerStack::update() {
        for (Layer* layer : m_overlayLayers){
            layer->onUpdate();
        }
        for (Layer* layer : m_layers){
            layer->onUpdate();
        }
    }


} // Ayla