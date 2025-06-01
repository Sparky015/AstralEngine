//
// Created by Andrew Fagan on 11/29/24.
//

#include "IndexBuffer.h"

#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/Platform/OpenGL/Resources/OpenGLIndexBuffer.h"
#include "Renderer/RHI/RendererCommands.h"

namespace Astral {
    IndexBufferHandle IndexBuffer::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        Device& device = g_RendererManager.GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::OpenGL: return CreateGraphicsRef<OpenGLIndexBuffer>(indices, count);
            case API::Vulkan: return device.CreateIndexBuffer(indices, count * sizeof(uint32));
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}