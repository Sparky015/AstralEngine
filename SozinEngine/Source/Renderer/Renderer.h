//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Renderer/Primitives/IndexBuffer.h"
#include "Renderer/Primitives/VertexBuffer.h"
#include "Renderer/Primitives/VertexArrayObject.h"

namespace Renderer {

    class Renderer
    {
    public:
        static void Submit(VertexArrayObject* vertexArrayBuffer);

    private:

    };

} // Renderer
