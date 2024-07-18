//
// Created by sparky on 7/17/2024.
//

#include "RendererLayer.h"

#include "Ayla/Renderer/ParseShaders.h"

#include "glad/glad.h"


namespace Ayla::Renderer {


    RendererLayer::RendererLayer() :
        m_Buffer(0),
        m_VertexShader(Shader("../../source/Ayla/Renderer/Shaders/vertex.shader").GetShaderCode()),
        m_FragmentShader(Shader("../../source/Ayla/Renderer/Shaders/fragment.shader").GetShaderCode())
    {
        AY_TRACE("[Sholas] RendererLayer: Initializing Renderer Layer");
        this->AttachLayer();
        m_DebugName = "Renderer Layer";
    }


    RendererLayer::~RendererLayer()
    {
        AY_TRACE("[Sholas] RendererLayer: Destroying Renderer Layer");
        OnDetach();
        this->DetachLayer();
    }


    void RendererLayer::OnAttach()
    {

        float positions[6] = {
            -0.5f, -0.5,
            0.0f, 0.5f,
            0.5f, -0.5f
        };

        unsigned int VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); // the secret magic sauce


        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (float*)positions, GL_STATIC_DRAW);

        m_Shader = CreateShader(m_VertexShader, m_FragmentShader);
        glUseProgram(m_Shader);
    }


    void RendererLayer::OnDetach()
    {
        glDeleteProgram(m_Shader);
    }


    void RendererLayer::OnUpdate()
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3);
    }


    Events::EEventCategory RendererLayer::GetAcceptingEventFlags()
    {
        return Ayla::Events::None;
    }


    unsigned int RendererLayer::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);



        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            AY_LOG_SS("Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex!" : "fragment!"));
            AY_LOG(message);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }


    unsigned int RendererLayer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs_id);
        glAttachShader(program, fs_id);

        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs_id);
        glDeleteShader(fs_id);

        return program;
    }



} // namespace Ayla::Renderer




