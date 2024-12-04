//
// Created by Andrew Fagan on 11/29/24.
//

#include "IndexBuffer.h"
#include "Renderer/Renderer.h"

#include "Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h"

namespace Renderer {

    IndexBuffer* IndexBuffer::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

} // Renderer