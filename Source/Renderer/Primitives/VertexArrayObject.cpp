//
// Created by Andrew Fagan on 11/30/24.
//

#include "VertexArrayObject.h"

#include "Renderer/Renderer.h"

#include "Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h"

namespace Renderer {

    VertexArrayObject* VertexArrayObject::CreateVertexArrayObject()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL: return new OpenGLVertexArrayObject();
            case RendererAPI::Vulkan: ERROR("Vulkan is not supported yet!");;
            case RendererAPI::DirectX12: ERROR("DirectX12 is not supported yet!");;
            case RendererAPI::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}