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

        [[nodiscard]] virtual uint32 GetID() const = 0;

        virtual void SetTextureUniform(const std::string& uniformName, int slot) = 0;

        virtual void SetUniform(const std::string& uniformName, float f1) const = 0;
        virtual void SetUniform(const std::string& uniformName, float f1, float f2) const = 0;
        virtual void SetUniform(const std::string& uniformName, float f1, float f2, float f3) const = 0;
        virtual void SetUniform(const std::string& uniformName, float f1, float f2, float f3, float f4) const = 0;
        virtual void SetUniform(const std::string& uniformName, int32 i1) const = 0;
        virtual void SetUniform(const std::string& uniformName, int32 i1, int32 i2) const = 0;
        virtual void SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3) const = 0;
        virtual void SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3, int32 i4) const = 0;
        virtual void SetUniform(const std::string& uniformName, uint32 ui1) const = 0;
        virtual void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2) const = 0;
        virtual void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3) const = 0;
        virtual void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3, uint32 ui4) const = 0;
        virtual void SetUniform(const std::string& uniformName, Mat4 mat4) const = 0;
        virtual void SetUniform(const std::string& uniformName, Vec4 vec4) const = 0;
        virtual void SetUniform(const std::string& uniformName, Vec3 vec3) const = 0;

        static ShaderProgram* CreateShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);

    private:
        virtual uint32 CompileShader(uint32 shaderType, const std::string& shaderSource) = 0;
    };

} // Renderer