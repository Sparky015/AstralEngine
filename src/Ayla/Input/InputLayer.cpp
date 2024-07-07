//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputLayer.h"
#include "Ayla/Events/Event.h"


namespace Ayla::Input {

    using namespace Events;

    InputLayer::InputLayer()
    {
        AY_TRACE("[Sholas] InputLayer: Initializing Input Layer");
        this->AttachLayer();
        m_DebugName = "Input Layer";
    }


    InputLayer::~InputLayer()
    {
        AY_TRACE("[Sholas] InputLayer: Destroying Input Layer");
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
