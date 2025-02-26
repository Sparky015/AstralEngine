//
// Created by Andrew Fagan on 11/29/24.
//

#include "IndexBuffer.h"

#include "Debug/Utils/Public/Error.h"
#include "Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    IndexBuffer* IndexBuffer::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLIndexBuffer(indices, count);
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer