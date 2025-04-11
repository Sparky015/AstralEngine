//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/Primitives/IndexBuffer.h"
#include "Renderer/Primitives/VertexBuffer.h"

#include "Renderer/Primitives/VertexArrayObject.h"
#include "Renderer/Shaders/ShaderProgram.h"
#include "Renderer/Cameras/OrthographicCamera.h"

namespace Astral {

    class Renderer
    {
    public:
        static void BeginScene(const OrthographicCamera& orthographicCamera);
        static void EndScene();

        static void Submit(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayBuffer, Mat4& transform);

        static void GetDrawCallsPerFrame();
    private:

    };

} // Renderer
