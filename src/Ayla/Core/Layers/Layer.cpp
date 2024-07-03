//
// Created by Andrew Fagan on 4/24/24.
//

#include "Layer.h"
#include "LayerStack.h"
#include "Ayla/Core/Application.h"

namespace Ayla::Core::Layers {

    Layer::Layer() {
        AY_TRACE("Layer: Initializing Base Class Layer\t\t");
        m_isEnabled = false;
        m_isInitializedInTower = false;
    }

    Layer::~Layer(){
        detachLayer();
    }

    void Layer::attachLayer() {
        if (!m_isInitializedInTower){
            Application::getApplication().getLayerStack().appendLayer(this);
            m_isInitializedInTower = true;
            onAttach();
            m_isEnabled = true;
        }
    }

    void Layer::attachOverlay() {
        if (!m_isInitializedInTower){

            Application::getApplication().getLayerStack().appendOverlay(this);
            m_isInitializedInTower = true;
            onAttach();
            m_isEnabled = true;
        }
    }

    void Layer::detachLayer() {
        if (m_isInitializedInTower) {
            Application::getApplication().getLayerStack().removeLayer(this);
            m_isInitializedInTower = false;
            onDetach();
            m_isEnabled = false;
        }
    }


    void Layer::detachOverlay() {
        if (m_isInitializedInTower) {
            Application::getApplication().getLayerStack().removeOverlay(this);
            m_isInitializedInTower = false;
            onDetach();
            m_isEnabled = false;
        }
    }

    bool Layer::isEnabled() {
        return m_isEnabled;
    }

    void Layer::enable() {
        m_isEnabled = true;
    }

    void Layer::disable() {
        m_isEnabled = false;
    }

    void Layer::setCallback(std::function<void(Event&)> callback) {
        m_callback = std::move(callback);
    }

//    void Layer::onEvent(Ayla::Events& event) {
//        m_callback(event);
//    }



} // Ayla