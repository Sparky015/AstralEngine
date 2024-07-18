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
    virtual void OnUpdate() override;
    virtual Events::EEventCategory GetAcceptingEventFlags() override;

    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

private:

    unsigned int m_Buffer;

    std::string m_VertexShader =
        "#version 410\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "}\n"
        "\n";

    std::string m_FragmentShader =
        "#version 410\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n"
        "\n";

};



}