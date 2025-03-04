//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "Renderer/AbstractionLayer/Primitives/VertexArrayObject.h"
#include "Renderer/AbstractionLayer/Primitives/ShaderProgram.h"
#include "AbstractionLayer/RendererAPI.h"

namespace Graphics {

    class RendererCommand
    {
    public:
        static void Clear();
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElements(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform);
        static void SetBlending(bool enable);

        static uint32 GetNumberOfDrawCallsPerFrame();

    private:
        static RendererAPI* s_RendererAPI;
    };



}