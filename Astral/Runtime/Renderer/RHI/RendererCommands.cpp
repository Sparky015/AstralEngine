/**
* @file RendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "RendererCommands.h"

namespace Astral {

#ifdef ASTRAL_DEFAULT_RENDERING_API_OPENGL
    API RendererCommands::s_RendererAPI = API::OpenGL;
#elif ASTRAL_DEFAULT_RENDERING_API_VULKAN
    API RendererCommands::s_RendererAPI = API::Vulkan;
#elif ASTRAL_DEFAULT_RENDERING_API_DIRECTX12
    API RendererCommands::s_RendererAPI = API::DirectX12;
#elif ASTRAL_DEFAULT_RENDERING_API_METAL
    API RendererCommands::s_RendererAPI = API::Metal;
#else
    API RendererCommands::s_RendererAPI = API::None;
#endif

    RendererCommands::RendererCommands()
    {
        m_NewFrameListener.StartListening();
    }

    RendererCommands::~RendererCommands()
    {
        m_NewFrameListener.StopListening();
    }
}
