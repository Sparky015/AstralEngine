//
// Created by Andrew Fagan on 11/29/24.
//

#include "VertexBuffer.h"

#include "Debug/Macros/Error.h"
#include "Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    VertexBuffer* VertexBuffer::CreateVertexBuffer(float* vertices, unsigned int size, const BufferLayout& bufferLayout)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLVertexBuffer(vertices, size, bufferLayout);
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer