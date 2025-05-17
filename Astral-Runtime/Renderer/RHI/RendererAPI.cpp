//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererCommands.h"
#include "Platform/Vulkan/VulkanRendererCommands.h"
#include "Resources/VertexArrayObject.h"


namespace Astral {

    RendererCommands* RendererAPI::s_RendererCommands = new VulkanRendererCommands();

    void RendererAPI::Clear()
    {
        //s_RendererCommands->Clear(TODO, TODO);
    }


    void RendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        s_RendererCommands->SetClearColor(r, g, b, a);
    }


    void RendererAPI::DrawElements(Shader& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform)
    {
        // shaderProgram.SetUniform("u_Transform", transform);
        // shaderProgram.Bind();
        // s_RendererCommands->DrawElements(vertexArrayObject);
    }


    void RendererAPI::SetBlending(bool enable)
    {
        s_RendererCommands->SetBlending(enable);
    }


    void RendererAPI::TestInit()
    {
        s_RendererCommands->TestInit();
    }


    void RendererAPI::TestUpdate()
    {
        s_RendererCommands->TestUpdate();
    }


    void RendererAPI::TestShutdown()
    {
        s_RendererCommands->TestShutdown();
    }

}
