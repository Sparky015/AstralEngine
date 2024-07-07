//
// Created by Andrew Fagan on 6/25/24.
//

#include "DebugLayer.h"
#include "Ayla/Input/InputState.h"
#include "Ayla/Input/Keycodes.h"
#include "Ayla/Core/Time/Clock.h"



namespace Ayla::Core::Debug
{

    DebugLayer::DebugLayer()
    {
        AY_TRACE("[Sholas] DebugLayer: Initializing Debug Layer");
        this->AttachLayer();
        m_DebugName = "Debug Layer";
    }


    DebugLayer::~DebugLayer()
    {
        AY_TRACE("[Sholas] DebugLayer: Destroying Debug Layer");
        this->DetachLayer();
    }


    void DebugLayer::OnAttach()
    {
        m_IsEnabled = true;
    }


    void DebugLayer::OnDetach()
    {
        m_IsEnabled = false;
    }


    void DebugLayer::OnUpdate()
    {
        using namespace Ayla::Input::Keycodes;

        static long long accumulatedTime;
        accumulatedTime += Time::Clock::Get().GetDeltaTime();

        Input::SInputState& inputState = Input::SInputState::get();
        if (inputState.isKeyDown(AY_KEY_T) && !inputState.isKeyRepeating(AY_KEY_T))
        {
            if (accumulatedTime <= 75){return;}
            accumulatedTime = 0;
            AY_TRACE("T is pressed down.");
            std::cout << "\n" << inputState.mousePositionX() << " by " << inputState.mousePositionY();
        }
        if (inputState.isKeyDown(AY_KEY_R) && !inputState.isKeyRepeating(AY_KEY_R))
        {
            if (accumulatedTime <= 300){return;}
            accumulatedTime = 0;
            AY_TRACE("Resetting stopwatch.");
            Ayla::Core::Time::Clock::Get().ResetStopwatch();
        }
    }


    void DebugLayer::OnEvent(IEvent& event)
    {
        // Not accepting events
    }


    EventCategory DebugLayer::GetAcceptingEventFlags()
    {
        return None;
    }


    void DebugLayer::OnImGuiRender()
    {
        // TODO: Make a debug window based on the keys pressed
    }

}




