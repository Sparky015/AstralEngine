//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

namespace Ayla::Renderer {

    class VertexBuffer
    {
    public:
        explicit VertexBuffer(void* data, unsigned int size);
        ~VertexBuffer();

        void Bind();
        void Unbind();

    private:
        unsigned int m_RendererID;
    };


}