//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "../../Astral-Runtime/Renderer/RHI/RendererAPI.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ChessEntities.h"
#include "MoveGeneration/KingChecks.h"

namespace Game {

    void GameManager::Init()
    {
        PROFILE_SCOPE("GameManager::Init");
        m_UpdateListener.StartListening();

        InitializeRenderingComponents();
        ChessEntities::InitEntities(m_Mesh);
    }


    void GameManager::Update()
    {
        PROFILE_SCOPE("GameManager::Update")
        m_PieceTracker.UpdatePieceTracking();
    }


    void GameManager::Shutdown()
    {
        TRACE("GameManager::Shutdown");
        m_UpdateListener.StopListening();

        m_Mesh.IndexBuffer.reset();
        m_Mesh.VertexBuffer.reset();

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
        PROFILE_SCOPE("GameManager::InitializeRenderingComponents")
        m_Mesh = {};

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

        {
            PROFILE_SCOPE("Compile Shaders")
            m_VertexBuffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout);
            m_IndexBuffer = IndexBuffer::CreateIndexBuffer(indices, 6);
        }

        m_Mesh.VertexBuffer = m_VertexBuffer;
        m_Mesh.IndexBuffer = m_IndexBuffer;
    }

}
