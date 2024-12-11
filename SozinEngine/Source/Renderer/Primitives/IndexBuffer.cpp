//
// Created by Andrew Fagan on 11/29/24.
//

#include "IndexBuffer.h"

#include "Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    IndexBuffer* IndexBuffer::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLIndexBuffer(indices, count);
            case API::Vulkan: ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

} // Renderer