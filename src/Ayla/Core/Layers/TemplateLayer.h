//
// Created by Andrew Fagan on 6/25/24.
//
#pragma once

#include "Layer.h"

namespace Ayla {

    class TemplateLayer : public Layer {
    public:
        TemplateLayer();
        ~TemplateLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event&) override;
        EventCategory getAcceptingEventFlags() override;

    private:

    };
}
