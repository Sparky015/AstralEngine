//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"
#include "RendererCommands.h"
#include "Shaders/ShaderSource.h"

#include "glad/glad.h" //TEMP

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

        RendererCommand::SetClearColor(.3,.3,.8,1);

        TRACE("Initializing Renderer Manager!");
        m_UpdateListener.StartListening();

        float vertices[15] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.0f,  0.5f, 0.0f, 0.5f, 1.0f
        };


        m_ShaderProgram.reset(ShaderProgram::CreateShaderProgram(ShaderSource(std::string(SHADER_DIR) + "basic.vert")
                , ShaderSource(std::string(SHADER_DIR) + "basic.frag")));
        m_ShaderProgram->Bind();

        uint32 indices[3] = { 0, 1, 2};

        BufferLayout bufferLayout = {
                {Float3, "a_Position"},
                {Float2, "a_TexCords"}
        };

        m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout));
        m_VAO.reset(VertexArrayObject::CreateVertexArrayObject());
        m_VAO->Bind();

        m_VAO->AddVertexBuffer(m_VertexBuffer.get());
        m_Texture.reset(Texture::CreateTexture(std::string(SHADER_DIR) + "../Resources/water.jpeg"));
        m_Texture->Bind();

        m_ShaderProgram->SetTextureUniform("u_Texture", 0);

        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 3));
        m_VAO->SetIndexBuffer(m_IndexBuffer.get());


        float g = sin(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        float r = cos(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        float b = cos(cos(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
        m_ShaderProgram->SetUniform("u_Color", r, g, b, 1.0f);

        GLCheckError();
    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE();


        RendererCommand::Clear();

//        unsigned int m_UniformLocation = glGetUniformLocation(m_ShaderProgram.GetID(), "u_Color");
//        ASSERT(m_UniformLocation != -1, "Uniform not found!");
//
//        float g = sin(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        float r = cos(sin(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        float b = cos(cos(static_cast<float>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count())));
//        glUniform4f(m_UniformLocation, r, g, b, 1.0f);

        RendererCommand::DrawElements(m_VAO.get());

        GLCheckError();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE();

        TRACE("Shutting down Renderer Manager!")
        m_UpdateListener.StopListening();

        // TODO: Fix this situation where member variable destructors are actually called.
        m_ShaderProgram.reset(nullptr);
        m_IndexBuffer.reset(nullptr);
        m_VertexBuffer.reset(nullptr);
        m_VAO.reset(nullptr);
        m_Texture.reset(nullptr);
//        RendererManager::~RendererManager();
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