/**
* @file OpenGLRendererAPI.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    class OpenGLRendererCommands : public RendererCommands
    {
        void Clear() override;
        void SetClearColor(float r, float g, float b, float a) override;
        void DrawElements(VertexArrayObject* vertexArrayObject) override;
        void SetBlending(bool enable) override;
    };

}