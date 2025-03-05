//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/RHI/Primitives/IndexBuffer.h"
#include "Renderer/RHI/Primitives/VertexBuffer.h"

#include "Renderer/RHI/Primitives/VertexArrayObject.h"
#include "Renderer/RHI/Primitives/ShaderProgram.h"
#include "Renderer/Cameras/OrthographicCamera.h"
#include "RHI/RendererAPI.h"

namespace Graphics {

    class Renderer
    {
    public:
        static void BeginScene(const OrthographicCamera& orthographicCamera);
        static void EndScene();

        static void Submit(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayBuffer, Mat4& transform);

        static uint32 GetDrawCallsPerFrame();
        static API GetRendererAPIBackend();

    };

} // Renderer
