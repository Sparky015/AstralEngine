//
// Created by Andrew Fagan on 11/29/24.
//

#include "VertexBuffer.h"
#include "Renderer/Renderer.h"

#include "Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h"

namespace Renderer {

    VertexBuffer* VertexBuffer::CreateVertexBuffer(float* vertices, unsigned int size, BufferLayout bufferLayout)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size, bufferLayout);
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
        }
    }

} // Renderer