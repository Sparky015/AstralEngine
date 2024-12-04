/**
* @file ShaderProgram.cpp
* @author Andrew Fagan
* @date 12/4/2024
*/
#include "ShaderProgram.h"
#include "Renderer/Renderer.h"
#include "Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h"

namespace Renderer {

    ShaderProgram* ShaderProgram::CreateShaderProgram(const ShaderSource &vertexShader,
                                                      const ShaderSource &fragmentShader)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL: return new OpenGLShaderProgram(vertexShader, fragmentShader);
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}
