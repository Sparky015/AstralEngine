//
// Created by Andrew Fagan on 7/18/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <string>
#include <filesystem>


namespace Astral {

    enum class ShaderType : uint8
    {
        NONE,
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
        GEOMETRY_SHADER,
        TESSELLATION_CONTROL_SHADER,
        TESSELLATION_EVALUATION_SHADER,
    };

    enum class ShaderLanguage : uint8
    {
        GLSL,
        HLSL
    };

    /** Parses a file to obtain a shader's source code and type from a file.*/
    class ShaderSource
    {
    public:
        ShaderSource();
        explicit ShaderSource(const std::filesystem::path& filePath);
        ~ShaderSource() = default;

        /** Parses shader code at a given relative path. */
        void ParseShaderCode(const std::filesystem::path& filePath);

        /** Returns the shader code in the form of a string */
        [[nodiscard]] const std::string& GetShaderCode() const;

        /** Retrieves the name of the file associated with the shader source.
         * @return A constant reference to the string containing the file name. */
        [[nodiscard]] const std::string& GetFileName() const;

        /** Returns the type of the shader */
        [[nodiscard]] ShaderType GetShaderType() const;

        /**
         * @brief Gets the source language of the shader
         * @return The source language of the shader
         */
        [[nodiscard]] ShaderLanguage GetShaderLanguage() const;

    private:

        /**
         * @brief Parses a shader file for its type, source language, and file name
         * @param filePath The file path to the shader file
         */
        void ParseShaderProperties(const std::filesystem::path& filePath);

        /** Parses shader code at a given relative path. */
        void ParseShaderInclude(const std::filesystem::path& filePath, std::string& outSource);

        std::string m_ShaderCode;
        std::string m_FileName;
        ShaderType m_ShaderType;
        ShaderLanguage m_ShaderLanguage;
    };



}
