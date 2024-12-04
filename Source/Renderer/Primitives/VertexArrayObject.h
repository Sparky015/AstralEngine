//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "VertexBuffer.h"

namespace Renderer {

    class VertexArrayObject
    {
    public:
        virtual ~VertexArrayObject() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void AddBuffer(VertexBuffer* vertexBuffer) = 0;

        static VertexArrayObject* CreateVertexArrayObject();
    };

} // Renderer