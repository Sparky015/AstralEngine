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
        static long long accumulatedTime;
        accumulatedTime += Time::Clock::get().getDeltaTime();
        if (InputState::get().access(AY_KEY_T).isDown && !InputState::get().access(AY_KEY_T).isRepeating){
            if (accumulatedTime <= 75){return;}
            accumulatedTime = 0;
            AY_TRACE("T is pressed down.");
            std::cout << "\n" << InputState::get().accessMouseX() << " by " << InputState::get().accessMouseY();
        }
        if (InputState::get().access(AY_KEY_R).isDown && !InputState::get().access(AY_KEY_R).isRepeating){
            if (accumulatedTime <= 300){return;}
            accumulatedTime = 0;
            AY_TRACE("Resetting stopwatch.");
            Ayla::Core::Time::Clock::get().resetStopwatch();
        }
    }

    void DebugLayer::onEvent(Event& event) {
        // Not accepting events
    }

    EventCategory DebugLayer::getAcceptingEventFlags() {
        return None;
    }

}




