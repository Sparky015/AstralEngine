//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla/Core/Layers/Layer.h"

using Ayla::Core::Layers::ILayer;

class ExampleLayer : public ILayer
{
public:
    ExampleLayer();
    ~ExampleLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnImGuiRender() override;
    void OnEvent(IEvent&) override;
    EventCategory GetAcceptingEventFlags() override;

private:

};