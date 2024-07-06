//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputLayer.h"



namespace Ayla::Input {
    InputLayer::InputLayer(){
        AY_TRACE("[Sholas] InputLayer: Initializing Input Layer");
        this->AttachLayer();
        m_DebugName = "Input Layer";
    }

    InputLayer::~InputLayer(){
        AY_TRACE("[Sholas] InputLayer: Initializing Input Layer");
        this->DetachLayer();
    }

    void InputLayer::OnAttach() {
        m_IsEnabled = true;
    }

    void InputLayer::OnDetach() {
        m_IsEnabled = false;
    }

    void InputLayer::OnUpdate() {

    }

    void InputLayer::OnEvent(Event& event) {
        //AY_LOG("Input State received an event!");
        m_Callback(event);
        //event.setIsHandled(false);
    }

    EventCategory InputLayer::GetAcceptingEventFlags() {
        return InputCategory;
    }

    void InputLayer::OnImGuiRender() {

    }

}
