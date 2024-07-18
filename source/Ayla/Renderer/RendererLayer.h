//
// Created by sparky on 7/17/2024.
//

#pragma once


#include "Ayla/Core/Layers/Layer.h"


namespace Ayla::Renderer{

class RendererLayer : public Core::Layers::ILayer
{
public:
    RendererLayer();
    ~RendererLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual Events::EEventCategory GetAcceptingEventFlags() override;

    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

private:

    unsigned int m_Buffer;
    unsigned int m_Shader;

    std::string m_VertexShader;
    std::string m_FragmentShader;


};



}