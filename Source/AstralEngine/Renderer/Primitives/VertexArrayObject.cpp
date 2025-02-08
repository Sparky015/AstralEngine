//
// Created by Andrew Fagan on 11/30/24.
//

#include "VertexArrayObject.h"

#include "Debug/Macros/Error.h"
#include "Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h"
#include "Renderer/RendererAPI.h"

namespace Graphics {

    VertexArrayObject* VertexArrayObject::CreateVertexArrayObject()
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLVertexArrayObject();
            case API::Vulkan: ERROR("Vulkan is not supported yet!");;
            case API::DirectX12: ERROR("DirectX12 is not supported yet!");;
            case API::Metal: ERROR("Metal is not supported yet!");
            default: ERROR("Invalid Renderer API");
        }
    }

}