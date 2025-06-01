//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Movement/PieceTracking.h"


#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Renderer/RHI/Resources/VertexArrayObject.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/RHI/Resources/Shader.h"

#include "Renderer/Common/Mesh.h"
#include "Renderer/Common/Material.h"


namespace Game {

    using namespace Astral;

    class GameManager : public SystemManager
    {
    public:
        GameManager();
        ~GameManager() override;

        void Init() override;
        void Update() override;
        void Shutdown() override;

        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;
        GameManager(GameManager&&) = delete;
        GameManager& operator=(GameManager&&) = delete;
    private:


        void InitializeRenderingComponents();

        Mesh m_Mesh;
        Material m_Material;

        IndexBufferHandle m_IndexBuffer;
        VertexBufferHandle m_VertexBuffer;
        ShaderHandle m_VertexShader;
        ShaderHandle m_FragmentShader;

        PieceTracking m_PieceTracker;
    };

}