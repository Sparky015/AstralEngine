/**
* @file Mesh.h
* @author Andrew Fagan
* @date 5/24/2025
*/


#pragma once

#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

namespace Astral {

    struct Mesh
    {
        VertexBufferHandle VertexBuffer;
        IndexBufferHandle IndexBuffer;
    };

}
