//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Renderer/Texture.h"

namespace Renderer {

    class OpenGLTexture : public Texture
    {
    public:
        explicit OpenGLTexture(const std::string& filePath);
        ~OpenGLTexture() override;

        void Bind(unsigned int slot) override;
        void Unbind() override;
        inline int GetWidth() override { return m_Width; };
        inline int GetHeight() override { return m_Height; };

    private:
        unsigned int m_RendererID;
        std::string filePath;
        unsigned char* m_ImageBuffer;
        int m_Width;
        int m_Height;
        int m_BPP; // bits per pixel
    };

} // Renderer
