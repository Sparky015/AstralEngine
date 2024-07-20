//
// Created by Andrew Fagan on 7/19/24.
//

#include "Renderer.h"

#include "glad/glad.h"

namespace Ayla::Renderer {

    IRenderer::IRenderer(const Shader& vertexShader, const Shader& fragmentShader) :
            m_ShaderProgram(ShaderProgram(vertexShader, fragmentShader)),
            m_VertexBufferData()
    {
        AY_TRACE("Initializing Renderer!");
    }

    IRenderer::~IRenderer()
    {
        AY_TRACE("Destroying Renderer!");
        glDeleteProgram(m_ShaderProgram.GetID());
    }


} // namespace Ayla::Renderer