//
// Created by Andrew Fagan on 7/18/24.
//

#include "ShaderSource.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Debug/Utilities/Loggers.h"

namespace Astral {


    ShaderSource::ShaderSource() :
        m_ShaderType(NONE)
    {}
    
    ShaderSource::ShaderSource(const std::filesystem::path& filePath) : m_ShaderType(NONE)
    {
        std::filesystem::path extension = filePath.extension();

        if (extension == ".vert") { m_ShaderType = VERTEX_SHADER; }
        else if (extension == ".frag") { m_ShaderType = FRAGMENT_SHADER; }
        else if (extension == ".comp") { m_ShaderType = COMPUTE_SHADER; }
        else if (extension == ".geom") { m_ShaderType = GEOMETRY_SHADER; }
        else if (extension == ".tesc") { m_ShaderType = TESSELLATION_CONTROL_SHADER; }
        else if (extension == ".tese") { m_ShaderType = TESSELLATION_EVALUATION_SHADER; }
        else { m_ShaderType = NONE; }

        m_FileName = filePath.filename().string();

        if (filePath.is_relative())
        {
            std::filesystem::path absolutePath = std::string(SHADER_DIR) + filePath.string();
            ParseShader(absolutePath);
        }
    }


    void ShaderSource::ParseShader(const std::filesystem::path& filePath)
    {
        if (!m_ShaderCode.empty())
        {
            WARN("Shader code already exists. Overwriting...");
        }

        std::ifstream file = std::ifstream(filePath);
        if (!file.is_open())
        {
            WARN("Failed to open file: " << filePath)
            return;
        }

        /** Accumulate all the shader source code into the stringstream. */
        std::stringstream ss;
        std::string line;
        while (std::getline(file, line))
        {
            ss << line << "\n";
        }

        m_ShaderCode = ss.str();
    }


    const std::string& ShaderSource::GetShaderCode() const
    {
        return m_ShaderCode;
    }


    const std::string& ShaderSource::GetFileName() const
    {
        return m_FileName;
    }


    ShaderType ShaderSource::GetShaderType() const
    {
        return m_ShaderType;
    }

}
