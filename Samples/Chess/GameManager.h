//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Movement/PieceTracking.h"


#include "Renderer/Primitives/IndexBuffer.h"
#include "Renderer/Primitives/VertexBuffer.h"
#include "Renderer/Primitives/VertexArrayObject.h"
#include "Renderer/Primitives/Texture.h"
#include "Renderer/Shaders/ShaderProgram.h"


namespace Game {

    using namespace Graphics;

    class GameManager : public SystemManager
    {
    public:
        static GameManager& Get();
        void Init() override;
        void Update() override;
        void Shutdown() override;

        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;
        GameManager(GameManager&&) = delete;
        GameManager& operator=(GameManager&&) = delete;
    private:
        GameManager();
        ~GameManager() override;

        void InitializeRenderingComponents();

        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<VertexArrayObject> m_VAO;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<ShaderProgram> m_ShaderProgram;

        PieceTracking m_PieceTracker;
    };

    extern GameManager& g_GameManager;

}