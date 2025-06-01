/**
* @file Shader.h
* @author Andrew Fagan
* @date 12/4/2024
*/

#pragma once

#include "Renderer/Shaders/ShaderSource.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Shader
    {
    public:
        virtual ~Shader() = default;

        static Shader* CreateShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
        static GraphicsRef<Shader> CreateShader(const ShaderSource& shaderSource);
        virtual void* GetNativeHandle() = 0;
    };

    using ShaderHandle = GraphicsRef<Shader>;

} // Renderer