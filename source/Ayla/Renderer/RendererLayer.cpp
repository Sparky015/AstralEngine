//
// Created by sparky on 7/17/2024.
//

#include "RendererLayer.h"

#include "glad/glad.h"


Ayla::Renderer::RendererLayer::RendererLayer() :m_Buffer(0)
{
    AY_TRACE("[Sholas] RendererLayer: Initializing Renderer Layer");
    this->AttachLayer();
    m_DebugName = "Renderer Layer";
}


Ayla::Renderer::RendererLayer::~RendererLayer()
{
    AY_TRACE("[Sholas] RendererLayer: Destroying Renderer Layer");
    OnDetach();
    this->DetachLayer();
}


void Ayla::Renderer::RendererLayer::OnAttach()
{
    float positions[6] = {
        -0.5f, -0.5,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    glGenBuffers(1, &m_Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (float*)positions, GL_STATIC_DRAW);
}


void Ayla::Renderer::RendererLayer::OnUpdate()
{
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);


    glDrawArrays(GL_TRIANGLES, 0, 3);
}


Ayla::Events::EEventCategory Ayla::Renderer::RendererLayer::GetAcceptingEventFlags()
{
    return Ayla::Events::None;
}




