//
// Created by Andrew Fagan on 11/30/24.
//

#include "OpenGLTexture.h"

#include "std_image.h"
#include "glad/glad.h"

namespace Renderer {

    OpenGLTexture::OpenGLTexture(const std::string& filePath) : m_ImageBuffer(nullptr), m_Height(0), m_Width(0), m_BPP(0), m_RendererID(0)
    {
//        stbi_set_flip_vertically_on_load(1);
        m_ImageBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
        if (!m_ImageBuffer) { WARN("Texture failed to load!"); return;}

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ImageBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

         stbi_image_free(m_ImageBuffer);
    }


    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RendererID);
    }


    void OpenGLTexture::Bind(unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }


    void OpenGLTexture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

} // Renderer