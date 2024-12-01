//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

namespace Renderer {

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        static VertexBuffer* CreateVertexBuffer(float* vertices, unsigned int size);
    };

} // Renderer
