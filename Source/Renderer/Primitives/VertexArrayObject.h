//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

namespace Renderer {

    class VertexArrayObject
    {
    public:
        virtual ~VertexArrayObject() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        static VertexArrayObject* CreateVertexArrayObject();
    };

} // Renderer