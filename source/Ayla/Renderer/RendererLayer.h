//
// Created by sparky on 7/17/2024.
//

#pragma once


#include "Ayla/Core/Layers/Layer.h"


namespace Ayla::Renderer{

class RendererLayer : public Core::Layers::ILayer
{
public:

    virtual void OnAttach() override;
    virtual void OnUpdate() override;

private:

    unsigned int m_Buffer;

};



}