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

        [[nodiscard]] unsigned int GetID() const override;
        void Bind() const override;
        void Unbind() const override;

        virtual void SetTextureUniform(const std::string& uniformName, int slot) override;

        void SetUniform(const std::string& uniformName, float f1) const override;
        void SetUniform(const std::string& uniformName, float f1, float f2) const override;
        void SetUniform(const std::string& uniformName, float f1, float f2, float f3) const override;
        void SetUniform(const std::string& uniformName, float f1, float f2, float f3, float f4) const override;
        void SetUniform(const std::string& uniformName, int32 i1) const override;
        void SetUniform(const std::string& uniformName, int32 i1, int32 i2) const override;
        void SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3) const override;
        void SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3, int32 i4) const override;
        void SetUniform(const std::string& uniformName, uint32 ui1) const override;
        void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2) const override;
        void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3) const override;
        void SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3, uint32 ui4) const override;

    private:

        /*
        * Compiles a shader and returns the ID to the shader.
        * @param type: The type of shader that is being compiled.
        * @param source: The source code of the shader in the form of a string
        * @return The id of the shader
        */
        uint32 CompileShader(uint32 shaderType, const std::string& shaderSource) override;

        std::string m_VertexShaderCode;
        std::string m_FragmentShaderCode;
        uint32 m_ShaderProgramID;
    };



}