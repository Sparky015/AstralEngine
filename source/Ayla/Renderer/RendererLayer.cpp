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
        InitializeGPUData();
    }


    void RendererLayer::OnDetach()
    {
        glDeleteProgram(m_ShaderProgram);
    }


    void RendererLayer::OnUpdate()
    {
        Render();
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



        int result = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length = 0;
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


    unsigned int RendererLayer::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
    {
        const unsigned int program = glCreateProgram();
        const unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertexShader);
        const unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs_id);
        glAttachShader(program, fs_id);

        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs_id);
        glDeleteShader(fs_id);

        return program;
    }


    void RendererLayer::InitializeGPUData()
    {
        float positions[8] = {
                -0.5f, -0.5,
                0.5f, -0.5f,
                -0.5f, 0.5f,
                0.5f, 0.5f
        };

        unsigned int indices[6] = {
                0, 1, 2,
                1, 3, 2
        };

        unsigned int VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); // the secret magic sauce


        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), (float*)positions, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_Ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), (unsigned int*)indices, GL_STATIC_DRAW);




        m_ShaderProgram = CreateShaderProgram(m_VertexShader, m_FragmentShader);
        glUseProgram(m_ShaderProgram);
    }


    void RendererLayer::Render() const
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }


} // namespace Ayla::Renderer




