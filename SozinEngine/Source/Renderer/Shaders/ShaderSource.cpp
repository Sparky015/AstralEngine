//
// Created by Andrew Fagan on 7/18/24.
//

#include "ShaderSource.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Renderer{


    ShaderSource::ShaderSource() :
        m_ShaderType(NONE)
    {}

    ShaderSource::ShaderSource(const std::string& filePath) : m_ShaderType(NONE)
    {
        ParseShader(filePath);
    }


    void ShaderSource::ParseShader(const std::string& filePath)
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

        /** Find the shader type */
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                /** We found "#shader in one of the lines */

                /** Find which type of shader is specified after "#shader" */
                if (line.find("vertex") != std::string::npos)
                {
                    m_ShaderType = VERTEX_SHADER;
                    break;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    m_ShaderType = FRAGMENT_SHADER;
                    break;
                }
                else
                {
                   WARN("Shader type not found!");
                }
            }
        }

        /** Accumulate all the shader source code into the stringstream. */
        std::stringstream ss;
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


    ShaderType ShaderSource::GetShaderType() const
    {
        return m_ShaderType;
    }

}
