//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/Primitives/VertexArrayObject.h"

namespace Renderer {

    class OpenGLVertexArrayObject : public VertexArrayObject
    {
    public:
        OpenGLVertexArrayObject();
        ~OpenGLVertexArrayObject() override;

        void Bind() override;
        void Unbind() override;

    private:
        unsigned int m_RendererID;
    };

} // namespace Renderer