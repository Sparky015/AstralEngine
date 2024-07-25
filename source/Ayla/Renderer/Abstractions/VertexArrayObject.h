//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once


namespace Ayla::Renderer {

    class VertexArrayObject {
    public:
        VertexArrayObject();
        ~VertexArrayObject();

        void Bind();
        void Unbind();
    private:
        unsigned int m_RendererID;

    };


} // namespace Ayla::Renderer