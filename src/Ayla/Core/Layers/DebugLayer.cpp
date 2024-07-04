//
// Created by Andrew Fagan on 6/25/24.
//

#include "DebugLayer.h"
#include "Ayla/Core/Application.h"
#include "Ayla/Input/InputState.h"
#include "Ayla/Input/Keycodes.h"
#include "Ayla/Core/Time/Time.h"

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
        if (InputState::get().isKeyDown(AY_KEY_T) && !InputState::get().isKeyRepeating(AY_KEY_T)){
            if (accumulatedTime <= 75){return;}
            accumulatedTime = 0;
            AY_TRACE("T is pressed down.");
            std::cout << "\n" << InputState::get().mousePositionX() << " by " << InputState::get().mousePositionY();
        }
        if (InputState::get().isKeyDown(AY_KEY_R) && !InputState::get().isKeyRepeating(AY_KEY_R)){
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

    void DebugLayer::onImGuiRender() {

    }

}




