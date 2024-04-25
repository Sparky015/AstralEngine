//
// Created by Andrew Fagan on 4/24/24.
//

#include "Layer.h"
#include <Ayla/Core/LayerTower.h>

#include <utility>

namespace Ayla {

    Layer::Layer() {
        m_isEnabled = false;
        m_isInitializedInTower = false;
    }

    Layer::~Layer(){
        detachLayer();
    }

    void Layer::attachLayer() {
        if (!m_isInitializedInTower){
            LayerTower::get().appendLayer(this);
            m_isInitializedInTower = true;
        } else {
            m_isEnabled = true;
        }
    }

    void Layer::detachLayer() {
        if (m_isInitializedInTower) {
            LayerTower::get().removeLayer(this);
            m_isInitializedInTower = false;
        } else {
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

    void Layer::onEvent(Ayla::Event& event) {
        m_callback(event);
    }

} // Ayla