//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/RHI/Resources/VertexArrayObject.h"

namespace Graphics {

    class OpenGLVertexArrayObject : public VertexArrayObject
    {
    public:
        OpenGLVertexArrayObject();
        ~OpenGLVertexArrayObject() override;

        void Bind() override;
        void Unbind() override;
        void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
        void SetIndexBuffer(IndexBuffer* indexBuffer) override;

        uint32_t GetElementCount() override;

    private:
        unsigned int m_RendererID;
        uint8 m_AttributeCounter;
        uint32 m_StrideSize;
        uint32 m_ElementCount;
    };

} // namespace Renderer