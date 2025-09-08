/**
* @file Shader.cpp
* @author Andrew Fagan
* @date 5/24/2025
*/

#include "Shader.h"

#include "Core/Engine.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererCommands.h"

namespace Astral {

    GraphicsRef<Shader> Shader::CreateShader(const ShaderSource& shaderSource)
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateShader(shaderSource);
            case API::DirectX12: AE_ERROR("DirectX12 is not supported yet!");
            case API::Metal: AE_ERROR("Metal is not supported yet!");
            default: AE_ERROR("Invalid Renderer API");
        }
    }
}
