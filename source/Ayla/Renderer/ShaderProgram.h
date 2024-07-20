//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Ayla/Renderer/ParseShaders.h"

namespace Ayla::Renderer {


    /** Takes two shaders and creates a program from them. */
    class ShaderProgram
    {
    public:
        explicit ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
        ~ShaderProgram() = default;

        [[nodiscard]] unsigned int GetID() const;

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