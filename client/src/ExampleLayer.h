//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla/Core/Layers/Layer.h"

class ExampleLayer : public Ayla::Core::Layers::ILayer
{
public:
    ExampleLayer();
    ~ExampleLayer() override;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Ayla::Events::IEvent& event) override;
    virtual Ayla::Events::EEventCategory GetAcceptingEventFlags() override;

};