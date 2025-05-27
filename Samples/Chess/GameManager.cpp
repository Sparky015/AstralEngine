//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "../../Astral-Runtime/Renderer/RHI/RendererAPI.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ChessEntities.h"
#include "MoveGeneration/KingChecks.h"

namespace Game {

    GameManager& g_GameManager = GameManager::Get();

    GameManager& GameManager::Get()
    {
        static GameManager s_GameManager;
        return s_GameManager;
    }


    void GameManager::Init()
    {
        TRACE("GameManager::Init");
        m_UpdateListener.StartListening();

        InitializeRenderingComponents();
        ChessEntities::InitEntities(m_Mesh, m_Material);
    }


    void GameManager::Update()
    {
        PROFILE_SCOPE("GameManager::Update")
        m_PieceTracker.UpdatePieceTracking();
        RenderingSystem::RenderEntities();
    }


    void GameManager::Shutdown()
    {
        TRACE("GameManager::Shutdown");
        m_UpdateListener.StopListening();

        m_IndexBuffer.reset();
        m_VertexBuffer.reset();
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
        m_Mesh = {};
        m_Material = {};

        RendererAPI::SetClearColor(.3,.3,.8,1);

        float vertices[20] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };

        uint32 indices[6] = { 0, 1, 2, 1, 3, 2};

        VertexBufferLayout bufferLayout = {
            {Float3, "a_Position"},
            {Float2, "a_TexCords"}
        };

        m_VertexBuffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout);
        m_IndexBuffer = IndexBuffer::CreateIndexBuffer(indices, 6);

        m_Mesh.VertexBuffer = m_VertexBuffer;
        m_Mesh.IndexBuffer = m_IndexBuffer;

        m_VertexShader = Shader::CreateShader(ShaderSource(std::string(SHADER_DIR) + "basic.vert"));
        m_FragmentShader = Shader::CreateShader(ShaderSource(std::string(SHADER_DIR) + "basic.frag"));

        m_Material.PixelShader = m_FragmentShader;
        m_Material.VertexShader = m_VertexShader;
    }

}
