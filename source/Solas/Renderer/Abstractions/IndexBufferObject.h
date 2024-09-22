//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once



void GLCheckError();


namespace Solas::Renderer {

    class IndexBufferObject {
    public:
        explicit IndexBufferObject(void* data, unsigned int size);
        ~IndexBufferObject();

        void Bind() const;
        void Unbind();

    private:
        unsigned int m_RendererID{};

    };

} // namespace Solas::Renderer