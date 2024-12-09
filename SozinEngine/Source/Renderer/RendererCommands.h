//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "Renderer/Primitives/VertexArrayObject.h"
#include "RendererAPI.h"
#include "Renderer/Shaders/ShaderProgram.h"

namespace Renderer {

    class RendererCommand
    {
    public:
        static void Clear();
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElements(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform);

    private:
        static RendererAPI* s_RendererAPI;
    };



}