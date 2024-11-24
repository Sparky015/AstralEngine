//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputLayer.h"
#include "Solas/Events/Event.h"


namespace Solas::Input {

    using namespace EventManagement;

    InputLayer::InputLayer()
    {
        TRACE("[Solas] InputLayer: Initializing Input Layer");
        this->AttachLayer();
        m_DebugName = "Input Layer";
    }


    InputLayer::~InputLayer()
    {
        TRACE("[Solas] InputLayer: Destroying Input Layer");
        OnDetach();
        this->DetachLayer();
    }


    void InputLayer::OnEvent(IEvent& event)
    {
        //AY_LOG("Input State received an event!");
        m_Callback(event);
    }


    EEventCategory InputLayer::GetAcceptingEventFlags()
    {
        return INPUT_CATEGORY;
    }

}
