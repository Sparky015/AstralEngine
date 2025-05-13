//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "../Renderer.h"
#include "Renderer/RHI/Resources/VertexArrayObject.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "RendererAPI.h"

namespace Astral {

    class RendererCommand
    {
    public:
        static void Clear();
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElements(Shader& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform);
        static void SetBlending(bool enable);


    private:
        friend Renderer;
        static RendererAPI* GetRendererAPIBackend() { return s_RendererAPI; }
        static RendererAPI* s_RendererAPI;
    };



}