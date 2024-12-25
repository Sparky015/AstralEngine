/**
* @file ShaderProgram.cpp
* @author Andrew Fagan
* @date 12/4/2024
*/
#include "ShaderProgram.h"

#include "Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    ShaderProgram* ShaderProgram::CreateShaderProgram(const ShaderSource &vertexShader,
                                                      const ShaderSource &fragmentShader)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLShaderProgram(vertexShader, fragmentShader);
            case API::Vulkan: ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}
