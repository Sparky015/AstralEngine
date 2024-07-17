//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla.h"

class ExampleLayer : public Ayla::Core::Layers::ILayer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() override;

    virtual void OnImGuiRender() override;
    virtual Ayla::Events::EEventCategory GetAcceptingEventFlags() override;

};