//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Solas.h"

class ExampleLayer : public Solas::Core::Layers::ILayer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() override;

    virtual void OnImGuiRender() override;
    virtual Solas::EventManagement::EEventCategory GetAcceptingEventFlags() override;
};