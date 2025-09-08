//
// Created by Andrew Fagan on 11/29/24.
//

#include "VertexBuffer.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    VertexBufferHandle VertexBuffer::CreateVertexBuffer(float* vertices, unsigned int size, VertexBufferLayout& bufferLayout)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateVertexBuffer(vertices, size, bufferLayout);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }

} // Renderer