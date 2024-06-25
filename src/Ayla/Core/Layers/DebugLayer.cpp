//
// Created by Andrew Fagan on 6/25/24.
//

#include "DebugLayer.h"
#include "Ayla/Core/Application.h"
#include "Ayla/Input/InputState.h"

using namespace Ayla::Input;

namespace Ayla::Core::Debug {



    DebugLayer::DebugLayer(){
        AY_TRACE("DebugLayer: Initializing Debug Layer");
        this->attachLayer();
        m_debugName = "Debug Layer";
    }

    DebugLayer::~DebugLayer(){
        this->detachLayer();
    }

    void DebugLayer::onAttach() {
        m_isEnabled = true;
    }

    void DebugLayer::onDetach() {
        m_isEnabled = false;
    }

    void DebugLayer::onUpdate() {
        if (InputState::get().access(AY_KEY_T).isDown && !InputState::get().access(AY_KEY_T).isRepeating){
            AY_TRACE("T is pressed down.");
        }
    }

    void DebugLayer::onEvent(Event& event) {
        // Not accepting events
    }

    EventCategory DebugLayer::getAcceptingEventFlags() {
        return None;
    }

}




