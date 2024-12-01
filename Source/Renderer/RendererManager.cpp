//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"
#include "glad/glad.h"
#include "Shaders/ShaderSource.h"

namespace Renderer {

    RendererManager& g_RendererManager = RendererManager::Get();

    RendererManager& RendererManager::Get()
    {
        static RendererManager s_RendererManager;
        return s_RendererManager;
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE();

        TRACE("Initializing Renderer Manager!");
        m_UpdateListener.StartListening();

        float vertices[15] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.0f,  0.5f, 0.0f, 0.5f, 1.0f
        };


        m_ShaderProgram = ShaderProgram(ShaderSource(std::string(SHADER_DIR) + "basic.vert")
                , ShaderSource(std::string(SHADER_DIR) + "basic.frag"));
        m_ShaderProgram.Bind();

        uint32 indices[3] = { 0, 1, 2};
        m_VAO.reset(VertexArrayObject::CreateVertexArrayObject());
        m_VAO->Bind();
        m_Texture.reset(Texture::CreateTexture(std::string(SHADER_DIR) + "../Resources/water.jpeg"));
        m_Texture->Bind();
        int textureUniformLocation = glGetUniformLocation(m_ShaderProgram.GetID(), "u_Texture");
        glUniform1i(textureUniformLocation, 0);

        m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices)));
        m_VertexBuffer->Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// Add an attribute to the vertex attrib array
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 3));
        m_IndexBuffer->Bind();


        unsigned int m_UniformLocation = glGetUniformLocation(m_ShaderProgram.GetID(), "u_Color");
        ASSERT(m_UniformLocation != -1, "Uniform not found!");

        float g = sin(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        float r = cos(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        float b = cos(cos(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        glUniform4f(m_UniformLocation, r, g, b, 1.0f);

        GLCheckError();
    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE();

        glClearColor(.3,.3,.8,1);
        glClear(GL_COLOR_BUFFER_BIT);

//        unsigned int m_UniformLocation = glGetUniformLocation(m_ShaderProgram.GetID(), "u_Color");
//        ASSERT(m_UniformLocation != -1, "Uniform not found!");
//
//        float g = sin(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        float r = cos(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        float b = cos(cos(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        glUniform4f(m_UniformLocation, r, g, b, 1.0f);

        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        GLCheckError();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE();

        TRACE("Shutting down Renderer Manager!")
        m_UpdateListener.StopListening();
    }


    RendererManager::RendererManager()
    {
        TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        TRACE("Destroying Renderer Manager!")
    }

}