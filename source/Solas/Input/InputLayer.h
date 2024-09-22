//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Core/Layers/Layer.h"

namespace Ayla::Input {

    class InputLayer : public Core::Layers::ILayer
    {
    public:
        InputLayer();
        ~InputLayer() override;

        /** Passes the events to the InputState for adjusting the Input State. */
        void OnEvent(EventManagement::IEvent& event) override;

        /** Accepts Input flags for changing the InputState */
        EventManagement::EEventCategory GetAcceptingEventFlags() override;

    private:

    };
}
