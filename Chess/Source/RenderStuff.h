//
// Created by Andrew Fagan on 12/9/24.
//

#pragma once

#include "Sozin.h"
#include "ECS/ECSManager.h"
#include "Renderer/Renderer.h"

namespace Renderer {

    class RenderStuff {
    public:
        void Init();
        void Update();
        void Shutdown();

    private:

        void InitEntities();
        Mat4 GetMouseTransform(Vec3 scale);
        ECS::Entity GetEntity(uint8 location);

        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<VertexArrayObject> m_VAO;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<ShaderProgram> m_ShaderProgram;


        ECS::Entity m_BlackPawn1;
        ECS::Entity m_BlackPawn2;
        ECS::Entity m_BlackPawn3;
        ECS::Entity m_BlackPawn4;
        ECS::Entity m_BlackPawn5;
        ECS::Entity m_BlackPawn6;
        ECS::Entity m_BlackPawn7;
        ECS::Entity m_BlackPawn8;
        ECS::Entity m_BlackRook1;
        ECS::Entity m_BlackRook2;
        ECS::Entity m_BlackKnight1;
        ECS::Entity m_BlackKnight2;
        ECS::Entity m_BlackBishop1;
        ECS::Entity m_BlackBishop2;
        ECS::Entity m_BlackQueen;
        ECS::Entity m_BlackKing;

        ECS::Entity m_WhitePawn1;
        ECS::Entity m_WhitePawn2;
        ECS::Entity m_WhitePawn3;
        ECS::Entity m_WhitePawn4;
        ECS::Entity m_WhitePawn5;
        ECS::Entity m_WhitePawn6;
        ECS::Entity m_WhitePawn7;
        ECS::Entity m_WhitePawn8;
        ECS::Entity m_WhiteRook1;
        ECS::Entity m_WhiteRook2;
        ECS::Entity m_WhiteKnight1;
        ECS::Entity m_WhiteKnight2;
        ECS::Entity m_WhiteBishop1;
        ECS::Entity m_WhiteBishop2;
        ECS::Entity m_WhiteQueen;
        ECS::Entity m_WhiteKing;

        ECS::Entity m_ChessBoard;

    };

}