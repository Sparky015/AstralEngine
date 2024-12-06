/**
* @file OpenGLRendererAPI.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Renderer/RendererAPI.h"

namespace Renderer {

    class OpenGLRendererAPI : public RendererAPI
    {
        void Clear() override;
        void SetClearColor(float r, float g, float b, float a) override;
        void DrawElements(VertexArrayObject* vertexArrayObject) override;
    };

}