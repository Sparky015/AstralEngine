//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "../SceneRenderer.h"
#include "Renderer/RHI/Resources/VertexArrayObject.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "RendererCommands.h"

namespace Astral {

    class RendererAPI
    {
    public:
        static void Clear();
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElements(Shader& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform);
        static void SetBlending(bool enable);

        static void TestInit();
        static void TestUpdate();
        static void TestShutdown();

    private:

        friend SceneRenderer;
        static RendererCommands* GetRendererAPIBackend() { return s_RendererCommands; }
        static RendererCommands* s_RendererCommands;
    };

}