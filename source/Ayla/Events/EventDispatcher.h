//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include "Ayla/Events/Event.h"


namespace Ayla::EventManagement{

    enum class EEventDispatchTypes : uint8_t
    {
        /** The event starts from the lowest system layer and moves through the Layer Stack to highest system layer (like UIs). */
        BACK_TO_FRONT,

        /** The event starts from the highest system layer and moves through the Layer Stack to lowest system layer. */
        FRONT_TO_BACK,

        /** The event gets pushed to only overlay layers. */
        PUSH_TO_OVERLAY,

        /** The event gets pushed to only basic layers. */
        PUSH_TO_LAYER
    };

    class EventDispatcher {
    public:


    private:



    };
} // namespace Ayla::EventManagement
