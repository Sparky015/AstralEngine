//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLShaderProgram.h"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>


namespace Graphics {

    OpenGLShaderProgram::OpenGLShaderProgram() : m_ShaderProgramID(0) {}

    OpenGLShaderProgram::OpenGLShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader) :
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


    unsigned int OpenGLShaderProgram::GetID() const
    {
        return m_ShaderProgramID;
    }


    void OpenGLShaderProgram::Bind() const
    {
        glUseProgram(m_ShaderProgramID);
    }


    void OpenGLShaderProgram::Unbind() const
    {
        glUseProgram(0);
    }


    uint32 OpenGLShaderProgram::CompileShader(uint32 shaderType, const std::string& shaderSource)
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
            WARN("Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!");
            WARN(message);
            glDeleteShader(shaderID);
            return 0;
        }

        /** Return the ID of the final shader form to be used in the program. */
        return shaderID;
    }


    void OpenGLShaderProgram::SetTextureUniform(const std::string& uniformName, int slot)
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform1i(uniformLocation, slot);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, float f1) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform1f(uniformLocation, f1);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, float f1, float f2) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform2f(uniformLocation, f1, f2);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, float f1, float f2, float f3) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform3f(uniformLocation, f1, f2, f3);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, float f1, float f2, float f3, float f4) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform4f(uniformLocation, f1, f2, f3, f4);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, int32 i1) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform1f(uniformLocation, i1);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, int32 i1, int32 i2) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform2f(uniformLocation, i1, i2);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform3i(uniformLocation, i1, i2, i3);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, int32 i1, int32 i2, int32 i3, int32 i4) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform4i(uniformLocation, i1, i2, i3, i4);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, uint32 ui1) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform1ui(uniformLocation, ui1);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform2ui(uniformLocation, ui1, ui2);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform3ui(uniformLocation, ui1, ui2, ui3);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, uint32 ui1, uint32 ui2, uint32 ui3,
                                         uint32 ui4) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform4ui(uniformLocation, ui1, ui2, ui3, ui4);
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, Mat4 mat4) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, Vec4 vec4) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vec4));
    }


    void OpenGLShaderProgram::SetUniform(const std::string& uniformName, Vec3 vec3) const
    {
        unsigned int uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        ASSERT(uniformLocation != -1, "Uniform name was not found in shader!");
        glUniform3fv(uniformLocation, 1, glm::value_ptr(vec3));
    }

} // namespace Renderer
