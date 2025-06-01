//
// Created by Andrew Fagan on 11/29/24.
//

#include "VertexBuffer.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/Platform/OpenGL/Resources/OpenGLVertexBuffer.h"
#include "Renderer/RHI/RendererCommands.h"

namespace Astral {
    VertexBufferHandle VertexBuffer::CreateVertexBuffer(float* vertices, unsigned int size,
                                                        VertexBufferLayout& bufferLayout)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::OpenGL: return CreateGraphicsRef<OpenGLVertexBuffer>(vertices, size, bufferLayout);
            case API::Vulkan: return device.CreateVertexBuffer(vertices, size, bufferLayout);
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

} // Renderer