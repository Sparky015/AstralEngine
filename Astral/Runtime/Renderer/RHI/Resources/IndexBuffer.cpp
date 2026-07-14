//
// Created by Andrew Fagan on 11/29/24.
//

#include "IndexBuffer.h"

#include "Core/Engine.h"
#include "Core/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    IndexBufferHandle IndexBuffer::CreateIndexBuffer(uint32* indexData, uint32 count, GPUMemoryType memoryType)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateIndexBuffer(indexData, count * sizeof(uint32), memoryType);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: return device.CreateIndexBuffer(indexData, count * sizeof(uint32), memoryType);
            default: AE_ERROR("Invalid Renderer API");
        }
    }

}