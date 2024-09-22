//
// Created by Andrew Fagan on 7/19/24.
//

#include "ShaderProgram.h"

#include "glad/glad.h"


namespace Solas::Renderer {

    ShaderProgram::ShaderProgram(const Solas::Renderer::Shader& vertexShader, const Solas::Renderer::Shader& fragmentShader) :
            m_VertexShaderCode(vertexShader.GetShaderCode()),
            m_FragmentShaderCode(fragmentShader.GetShaderCode()),
            m_ShaderProgramID(glCreateProgram())
    {
        const unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER, m_VertexShaderCode);
        const unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, m_FragmentShaderCode);

        /** Attach the two shaders to the program */
        glAttachShader(m_ShaderProgramID, vertexShaderID);
        glAttachShader(m_ShaderProgramID, fragmentShaderID);

        /** Link and validate the program. */
        glLinkProgram(m_ShaderProgramID);
        glValidateProgram(m_ShaderProgramID);

        /** Delete the shaders now that we are done with making the program. */
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }


    unsigned int ShaderProgram::GetID() const
    {
        return m_ShaderProgramID;
    }


    unsigned int ShaderProgram::CompileShader(unsigned int shaderType, const std::string& shaderSource)
    {
        /** Create a shader and get the shader ID. */
        const unsigned int shaderID = glCreateShader(shaderType);

        /** Convert the std::string into a usable C string. */
        const char* shaderSourceCode = shaderSource.c_str();

        /** Give OpenGL the shader code for the shader we created earlier and compile it. */
        glShaderSource(shaderID, 1, &shaderSourceCode, nullptr);
        glCompileShader(shaderID);


        /** Log errors if any. */
        int result = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(shaderID, length, &length, message);
            AY_LOG_SS("Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex!" : "fragment!"));
            AY_LOG(message);
            glDeleteShader(shaderID);
            return 0;
        }

        /** Return the ID of the final shader form to be used in the program. */
        return shaderID;
    }


} // namespace Solas::Renderer
