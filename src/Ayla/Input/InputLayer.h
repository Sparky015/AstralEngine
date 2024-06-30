//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once


#include "Ayla/Core/Layers/Layer.h"

using namespace Ayla::Events;
using namespace Ayla::Core::Layers;

namespace Ayla::Input {

    class InputLayer : public Layer {
    public:
        InputLayer();
        ~InputLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event&) override;
        EventCategory getAcceptingEventFlags() override;

    private:

    };
}
