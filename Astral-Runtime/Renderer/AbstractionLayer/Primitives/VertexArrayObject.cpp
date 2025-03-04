//
// Created by Andrew Fagan on 11/30/24.
//

#include "VertexArrayObject.h"

#include "Debug/Utilities/Error.h"
#include "Renderer/AbstractionLayer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h"
#include "Renderer/AbstractionLayer/RendererAPI.h"

namespace Graphics {

    VertexArrayObject* VertexArrayObject::CreateVertexArrayObject()
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: return new OpenGLVertexArrayObject();
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");;
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");;
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}