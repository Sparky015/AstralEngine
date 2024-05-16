//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputLayer.h"

namespace Ayla {
    InputLayer::InputLayer(){
        this->attachLayer();
    }

    InputLayer::~InputLayer(){
        this->detachLayer();
    }

    void InputLayer::onAttach() {
        m_isEnabled = true;
    }

    void InputLayer::onDetach() {
        m_isEnabled = false;
    }

    void InputLayer::onUpdate() {

    }

    void InputLayer::onEvent(Event& event) {
        m_callback(event);
    }

    EventCategory InputLayer::getAcceptingEventFlags() {
        return InputCategory;
    }

}
