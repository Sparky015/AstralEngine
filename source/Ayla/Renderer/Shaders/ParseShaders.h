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

    /** Parses a file to obtain a shader's source code and type from a file.*/
    class Shader
    {
    public:
        Shader();
        explicit Shader(const std::string& filePath);
        ~Shader() = default;

        /** Parses shader code at a given relative path. */
        void ParseShader(const std::string& filePath);

        /** Returns the shader code in the form of a string */
        [[nodiscard]] const std::string& GetShaderCode() const;

        /** Returns the type of the shader */
        [[nodiscard]] ShaderType GetShaderType() const;

    private:
        std::string m_ShaderCode;
        ShaderType m_ShaderType;
    };



}