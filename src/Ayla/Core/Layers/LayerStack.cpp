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

    void LayerStack::appendLayer(ILayer* layer) {
        m_layers.push_back(layer);
    }

    void LayerStack::insertLayer(ILayer* layer, int index) {

    }

    void LayerStack::removeLayer(ILayer* layer) {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }

    void LayerStack::appendOverlay(ILayer* layer) {
        m_overlayLayers.push_back(layer);
    }

    void LayerStack::insertOverlay(ILayer* layer, int index) {

    }

    void LayerStack::removeOverlay(ILayer* layer) {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }

    void LayerStack::dispatchEventBackToFront(Event& event) {
        for (ILayer* layer : m_layers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {continue;}
            layer->OnEvent(event);
        }
        for (ILayer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {continue;}
            layer->OnEvent(event);
        }
    }

    void LayerStack::dispatchEventFromFrontToBack(Event& event) {
        for (ILayer* layer : m_overlayLayers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }
        for (ILayer* layer : m_layers){
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }

    }

    void LayerStack::update() {
        for (ILayer* layer : m_overlayLayers){
            layer->OnUpdate();
        }
        for (ILayer* layer : m_layers){
            layer->OnUpdate();
        }
    }

    void LayerStack::renderImGui(){
        for (ILayer* layer : m_layers){
            layer->OnImGuiRender();
        }
        for (ILayer* layer : m_overlayLayers){
            layer->OnImGuiRender();
        }
    }


} // Ayla