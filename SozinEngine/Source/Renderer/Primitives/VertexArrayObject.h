//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Graphics {

    class VertexArrayObject
    {
    public:
        virtual ~VertexArrayObject() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
        virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

        virtual uint32 GetElementCount() = 0;

        static VertexArrayObject* CreateVertexArrayObject();
    };

} // Renderer