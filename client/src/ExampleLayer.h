//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla/Core/Layers/Layer.h"

using Ayla::Core::Layers::Layer;

class ExampleLayer : public Layer {
public:
    ExampleLayer();
    ~ExampleLayer() override;

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onImGuiRender() override;
    void onEvent(Event&) override;
    EventCategory getAcceptingEventFlags() override;

private:

};