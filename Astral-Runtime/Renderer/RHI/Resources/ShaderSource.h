//
// Created by Andrew Fagan on 7/18/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <string>
#include <filesystem>


namespace Astral {

    enum ShaderType : uint8
    {
        NONE,
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
        GEOMETRY_SHADER,
        TESSELLATION_CONTROL_SHADER,
        TESSELLATION_EVALUATION_SHADER,
    };

    /** Parses a file to obtain a shader's source code and type from a file.*/
    class ShaderSource
    {
    public:
        ShaderSource();
        explicit ShaderSource(const std::filesystem::path& filePath);
        ~ShaderSource() = default;

        /** Parses shader code at a given relative path. */
        void ParseShader(const std::filesystem::path& filePath);

        /** Returns the shader code in the form of a string */
        [[nodiscard]] const std::string& GetShaderCode() const;

        /** Retrieves the name of the file associated with the shader source.
         * @return A constant reference to the string containing the file name. */
        [[nodiscard]] const std::string& GetFileName() const;

        /** Returns the type of the shader */
        [[nodiscard]] ShaderType GetShaderType() const;

    private:

        /** Parses shader code at a given relative path. */
        void ParseShaderInclude(const std::filesystem::path& filePath, std::string& outSource);

        std::string m_ShaderCode;
        std::string m_FileName;
        ShaderType m_ShaderType;
    };



}
