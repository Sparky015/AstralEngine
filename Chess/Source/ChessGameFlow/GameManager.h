//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "PlayerInput/PieceTracking.h"
// TODO: Fix the header dependencies. Somehow, graphics classes are being included from PieceTracking.h. Look into Dependabot(?)
#include "Renderer/Renderer.h"

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