//
// Created by Andrew Fagan on 11/29/24.
//

#include "VertexBuffer.h"

#include "Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h"
#include "Renderer/RendererAPI.h"

namespace Renderer {

    VertexBuffer* VertexBuffer::CreateVertexBuffer(float* vertices, unsigned int size, const BufferLayout& bufferLayout)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLVertexBuffer(vertices, size, bufferLayout);
            case API::Vulkan: ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

} // Renderer