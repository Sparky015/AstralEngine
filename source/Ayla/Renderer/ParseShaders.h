//
// Created by Andrew Fagan on 7/18/24.
//

#pragma once

namespace Ayla::Renderer {

    enum ShaderType : uint8
    {
        NONE,
        VERTEX_SHADER,
        FRAGMENT_SHADER,
    };

    class Shader
    {
    public:
        Shader();
        explicit Shader(const std::string& filePath);
        ~Shader() = default;

        /** Parses shader code at a given relative path. */
        void ParseShader(const std::string& filePath);

        /** Returns the shader code in the form of a string */
        std::string& GetShaderCode();

        /** Returns the type of the shader */
        ShaderType GetShaderType();

    private:
        std::string m_ShaderCode;
        ShaderType m_ShaderType;
    };



}