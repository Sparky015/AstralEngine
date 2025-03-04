//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/AbstractionLayer/Primitives/IndexBuffer.h"
#include "Renderer/AbstractionLayer/Primitives/VertexBuffer.h"

#include "Renderer/AbstractionLayer/Primitives/VertexArrayObject.h"
#include "Renderer/AbstractionLayer/Primitives/ShaderProgram.h"
#include "Renderer/Cameras/OrthographicCamera.h"

namespace Graphics {

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
