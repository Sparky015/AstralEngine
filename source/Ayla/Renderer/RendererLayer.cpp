//
// Created by sparky on 7/17/2024.
//

#include "RendererLayer.h"

#include "glad/glad.h"


void Ayla::Renderer::RendererLayer::OnAttach()
{
    float positions[6] = {
        -0.5f, -0.5,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    glGenBuffers(1, &m_Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (float*)positions, GL_STATIC_DRAW);
}


void Ayla::Renderer::RendererLayer::OnUpdate()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
