//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputLayer.h"



namespace Ayla::Input {
    InputLayer::InputLayer(){
        AY_TRACE("[Sholas] InputLayer: Initializing Input Layer");
        this->attachLayer();
        m_debugName = "Input Layer";
    }

    InputLayer::~InputLayer(){
        AY_TRACE("[Sholas] InputLayer: Initializing Input Layer");
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
        //AY_LOG("Input State received an event!");
        m_callback(event);
        //event.setIsHandled(false);
    }

    EventCategory InputLayer::getAcceptingEventFlags() {
        return InputCategory;
    }

    void InputLayer::onImGuiRender() {

    }

}
