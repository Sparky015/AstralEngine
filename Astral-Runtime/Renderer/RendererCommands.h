//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "Renderer.h"
#include "Renderer/RHI/Primitives/VertexArrayObject.h"
#include "Renderer/RHI/Primitives/ShaderProgram.h"
#include "RHI/RendererAPI.h"

namespace Graphics {

    class RendererCommand
    {
    public:
        static void Clear();
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElements(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform);
        static void SetBlending(bool enable);


    private:
        friend Renderer;
        static RendererAPI* GetRendererAPIBackend() { return s_RendererAPI; }
        static RendererAPI* s_RendererAPI;
    };



}