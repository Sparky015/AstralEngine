//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    class OpenGLTexture : public Texture
    {
    public:
        explicit OpenGLTexture(const std::filesystem::path& filePath);
        ~OpenGLTexture() override;

        void Bind(unsigned int slot) override;
        void Unbind() override;
        int GetWidth() override { return m_Width; }
        int GetHeight() override { return m_Height; }
        bool IsValid() override { return m_IsValid && m_RendererID != 0; }

        void* GetNativeHandle() override { return nullptr; }
        void* GetSampler() override { return nullptr; }

    private:
        unsigned int m_RendererID;
        std::string filePath;
        unsigned char* m_ImageBuffer;
        int m_Width;
        int m_Height;
        int m_BPP; // bits per pixel
        bool m_IsValid;
    };

} // Renderer
