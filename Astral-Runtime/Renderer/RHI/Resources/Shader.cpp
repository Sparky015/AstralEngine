/**
* @file ShaderProgram.cpp
* @author Andrew Fagan
* @date 12/4/2024
*/
#include "Shader.h"

#include "Debug/Utilities/Error.h"
#include "Renderer/RHI/Platform/OpenGL/Resources/OpenGLShaderProgram.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    Shader* Shader::CreateShaderProgram(const ShaderSource &vertexShader,
                                                      const ShaderSource &fragmentShader)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLShaderProgram(vertexShader, fragmentShader);
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}
