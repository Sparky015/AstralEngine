//
// Created by Andrew Fagan on 6/25/24.
//
#pragma once

#include "Ayla/Core/Layers/Layer.h"

using namespace Ayla::Core::Layers;

namespace Ayla::Core::Debug {

    class DebugLayer : public Layer {
    public:
        DebugLayer();
        ~DebugLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event&) override;
        EventCategory getAcceptingEventFlags() override;

    private:

    };

}