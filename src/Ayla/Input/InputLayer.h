//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once


#include "Ayla/Core/Layers/Layer.h"

using namespace Ayla::Events;
using namespace Ayla::Core::Layers;

namespace Ayla::Input {

    class InputLayer : public ILayer {
    public:
        InputLayer();
        ~InputLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnImGuiRender() override;
        void OnEvent(IEvent&) override;
        EventCategory GetAcceptingEventFlags() override;

    private:

    };
}
