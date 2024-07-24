//
// Created by Andrew Fagan on 6/25/24.
//
#pragma once

#include "Ayla/Core/Layers/Layer.h"

#include "Ayla/Events/EventListener.h"
#include "Ayla/Events/KeyEvent.h"


namespace Ayla::Core::Debug
{
    using namespace Ayla::Core::Layers;
    /** Provides a way to debug the engine by pulling information into a window or the console for viewing. */
    class DebugLayer : public ILayer
    {
    public:
        DebugLayer();
        ~DebugLayer() override;

        /** Checks if keys are pressed, so we know when to show debugging info */
        void OnUpdate() override;

        /** A space to create the ImGui windows to show debugging info. */
        void OnImGuiRender() override;

        /** The Debug Layer does not accept events. */
        EEventCategory GetAcceptingEventFlags() override;

        void OnKeyPressEvent(KeyPressedEvent& event);
    private:
        EventManagement::EventListener<KeyPressedEvent&> m_KeyPressedListener{[this](KeyPressedEvent& e){ this->OnKeyPressEvent(e);}};
        static bool m_ShowImGuiDemoWindow;
        static bool m_ShowDebugMenu;
    };

}