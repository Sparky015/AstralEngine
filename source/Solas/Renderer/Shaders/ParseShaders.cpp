//
// Created by Andrew Fagan on 7/18/24.
//

#include "ParseShaders.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Solas::Renderer{


    Shader::Shader() :
        m_ShaderType(NONE)
    {}

    Shader::Shader(const std::string& filePath) : m_ShaderType(NONE)
    {
        ParseShader(filePath);
    }


    void Shader::ParseShader(const std::string& filePath)
    {
        if (!m_ShaderCode.empty())
        {
            AY_WARN("Shader code already exists. Overwriting...");
        }

        std::ifstream file = std::ifstream(filePath);
        if (!file.is_open())
        {
            AY_WARN_SS("[" << __FILE__ << "] " << "Failed to open file: " << filePath)
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
                   AY_ERROR("Shader type not found!");
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


    const std::string& Shader::GetShaderCode() const
    {
        return m_ShaderCode;
    }


    ShaderType Shader::GetShaderType() const
    {
        return m_ShaderType;
    }

}
