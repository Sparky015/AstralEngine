//
// Created by Andrew Fagan on 7/19/24.
//

#include "Renderer.h"

#include "glad/glad.h"

namespace Solas::Renderer {

    IRenderer::IRenderer(const Shader& vertexShader, const Shader& fragmentShader) :
            m_ShaderProgram(ShaderProgram(vertexShader, fragmentShader)),
            m_VertexBufferData()
    {
        TRACE("Initializing Renderer!");
    }

    IRenderer::~IRenderer()
    {
        TRACE("Destroying Renderer!");
        glDeleteProgram(m_ShaderProgram.GetID());
    }


} // namespace Solas::Renderer