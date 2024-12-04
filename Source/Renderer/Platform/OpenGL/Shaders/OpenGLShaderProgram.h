//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/Shaders/ShaderSource.h"

#include "Renderer/Shaders/ShaderProgram.h"

namespace Renderer {

    /** Takes two shaders and creates a program from them. */
class OpenGLShaderProgram : public ShaderProgram
    {
    public:
        OpenGLShaderProgram();
        explicit OpenGLShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
        ~OpenGLShaderProgram() override = default;

        [[nodiscard]] unsigned int GetID() const;
        void Bind();
        void Unbind();

        void SetUniform(std::string&& uniformName, float f1) const;
        void SetUniform(std::string&& uniformName, float f1, float f2) const;
        void SetUniform(std::string&& uniformName, float f1, float f2, float f3) const;
        void SetUniform(std::string&& uniformName, float f1, float f2, float f3, float f4) const;
        void SetUniform(std::string&& uniformName, int32 i1) const;
        void SetUniform(std::string&& uniformName, int32 i1, int32 i2) const;
        void SetUniform(std::string&& uniformName, int32 i1, int32 i2, int32 i3) const;
        void SetUniform(std::string&& uniformName, int32 i1, int32 i2, int32 i3, int32 i4) const;
        void SetUniform(std::string&& uniformName, uint32 ui1) const;
        void SetUniform(std::string&& uniformName, uint32 ui1, uint32 ui2) const;
        void SetUniform(std::string&& uniformName, uint32 ui1, uint32 ui2, uint32 ui3) const;
        void SetUniform(std::string&& uniformName, uint32 ui1, uint32 ui2, uint32 ui3, uint32 ui4) const;

    private:

        /*
        * Compiles a shader and returns the ID to the shader.
        * @param type: The type of shader that is being compiled.
        * @param source: The source code of the shader in the form of a string
        * @return The id of the shader
        */
        unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSource);

        std::string m_VertexShaderCode;
        std::string m_FragmentShaderCode;
        unsigned int m_ShaderProgramID;
    };



}