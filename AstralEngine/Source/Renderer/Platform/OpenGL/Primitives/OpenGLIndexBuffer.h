//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/Primitives/IndexBuffer.h"

namespace Graphics {

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        explicit OpenGLIndexBuffer(uint32* indices, uint32 count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        uint32 GetCount() const override;

    private:
        unsigned int m_RendererID;
        uint32 m_Count;
    };

} // namespace Renderer