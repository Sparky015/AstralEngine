/**
* @file ShaderProgram.h
* @author Andrew Fagan
* @date 12/4/2024
*/

#pragma once

#include "ShaderSource.h"

namespace Renderer {

    class ShaderProgram
    {
    public:
        virtual ~ShaderProgram() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        [[nodiscard]] virtual uint32 GetCount() const = 0;

        static ShaderProgram* CreateShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
    };

} // Renderer