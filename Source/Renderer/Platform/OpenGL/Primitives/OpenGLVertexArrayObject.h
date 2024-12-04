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
        void AddBuffer(VertexBuffer* vertexBuffer) override;

    private:
        unsigned int m_RendererID;
        uint8 m_AttributeCounter;
        uint32 m_StrideSize;
    };

} // namespace Renderer