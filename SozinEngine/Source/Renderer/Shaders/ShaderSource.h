//
// Created by Andrew Fagan on 7/18/24.
//

#pragma once

namespace Renderer {

    enum ShaderType : uint8
    {
        NONE,
        VERTEX_SHADER,
        FRAGMENT_SHADER,
    };

    /** Parses a file to obtain a shader's source code and type from a file.*/
    class ShaderSource
    {
    public:
        ShaderSource();
        explicit ShaderSource(const std::string& filePath);
        ~ShaderSource() = default;

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