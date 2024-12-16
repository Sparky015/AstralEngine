//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "Renderer/RendererCommands.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ChessGameState/ChessEntities.h"

namespace Game {

    GameManager& g_GameManager = GameManager::Get();

    GameManager& GameManager::Get()
    {
        static GameManager s_GameManager;
        return s_GameManager;
    }


    void GameManager::Init()
    {
        TRACE("Initializing Game Manager");
        m_UpdateListener.StartListening();

        InitializeRenderingComponents();
        ChessEntities::InitEntities(m_VAO.get());
    }


    void GameManager::Update()
    {
        RendererCommand::Clear();
        m_PieceTracker.UpdatePieceTracking();
        Sozin::RenderingSystem::RenderEntities(m_ShaderProgram.get());
    }


    void GameManager::Shutdown()
    {
        TRACE("Shutting down Game Manager");
        m_UpdateListener.StopListening();

        m_ShaderProgram.reset(nullptr);
        m_IndexBuffer.reset(nullptr);
        m_VertexBuffer.reset(nullptr);
        m_VAO.reset(nullptr);
        m_Texture.reset(nullptr);
    }


    GameManager::GameManager()
    {
        TRACE("Constructing Game Manager");
    }


    GameManager::~GameManager()
    {
        TRACE("Destroying Game Manager");
    }


    void GameManager::InitializeRenderingComponents()
    {
        RendererCommand::SetClearColor(.3,.3,.8,1);

        float vertices[20] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };


        m_ShaderProgram.reset(ShaderProgram::CreateShaderProgram(ShaderSource(std::string(SHADER_DIR) + "basic.vert")
                , ShaderSource(std::string(SHADER_DIR) + "basic.frag")));
        m_ShaderProgram->Bind();

        uint32 indices[6] = { 0, 1, 2, 1, 3, 2};

        BufferLayout bufferLayout = {
                {Float3, "a_Position"},
                {Float2, "a_TexCords"}
        };

        m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout));
        m_VAO.reset(VertexArrayObject::CreateVertexArrayObject());
        m_VAO->Bind();

        m_VAO->AddVertexBuffer(m_VertexBuffer.get());

        m_ShaderProgram->SetTextureUniform("u_Texture", 0);

        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 6));
        m_VAO->SetIndexBuffer(m_IndexBuffer.get());

        RendererCommand::SetBlending(true);
    }

}
