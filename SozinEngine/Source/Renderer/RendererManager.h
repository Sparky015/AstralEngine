//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Renderer.h"

#include "Renderer/Primitives/VertexBuffer.h"
#include "Renderer/Primitives/IndexBuffer.h"
#include "Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "Renderer/Primitives/VertexArrayObject.h"
#include "Renderer/Primitives/Texture.h"

#include "ECS/Entity.h"


namespace Renderer {
    class RendererManager : public SystemManager
    {
    public:
        static RendererManager& Get();
        void Init() override;
        void Update() override;
        void Shutdown() override;

        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

    private:
        RendererManager();
        ~RendererManager() override;
    };

    extern RendererManager& g_RendererManager;
}