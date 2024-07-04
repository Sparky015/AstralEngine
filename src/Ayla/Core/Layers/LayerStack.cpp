//
// Created by Andrew Fagan on 4/24/24.
//

#include "LayerStack.h"

namespace Ayla::Core::Layers {

    LayerStack::LayerStack() {
        AY_TRACE("[Sholas] LayerStack: Initializing Layer Stack");
    }

    LayerStack::~LayerStack() {
        AY_TRACE("[Sholas] LayerStack: Destroying Layer Stack");
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

    void LayerStack::dispatchEventBackToFront(Event& event) {
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->getAcceptingEventFlags())) {continue;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->getAcceptingEventFlags())) {continue;}
            layer->onEvent(event);
        }
    }

    void LayerStack::dispatchEventFromFrontToBack(Event& event) {
        for (Layer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->getAcceptingEventFlags())) {return;}
            layer->onEvent(event);
        }
        for (Layer* layer : m_layers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->getAcceptingEventFlags())) {return;}
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

    void LayerStack::renderImGui(){
        for (Layer* layer : m_layers){
            layer->onImGuiRender();
        }
        for (Layer* layer : m_overlayLayers){
            layer->onImGuiRender();
        }
    }


} // Ayla