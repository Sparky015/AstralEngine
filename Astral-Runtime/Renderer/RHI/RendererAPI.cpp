/**
* @file RendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "RendererAPI.h"

namespace Astral {

#ifdef ASTRAL_DEFAULT_RENDERING_API_OPENGL
    API RendererAPI::s_RendererAPI = API::OpenGL;
#elif ASTRAL_DEFAULT_RENDERING_API_VULKAN
    API RendererAPI::s_RendererAPI = API::Vulkan;
#elif ASTRAL_DEFAULT_RENDERING_API_DIRECTX12
    API RendererAPI::s_RendererAPI = API::DirectX12;
#elif ASTRAL_DEFAULT_RENDERING_API_METAL
    API RendererAPI::s_RendererAPI = API::Metal;
#else
    API RendererAPI::s_RendererAPI = API::None;
#endif

    RendererAPI::RendererAPI()
    {
        m_NewFrameListener.StartListening();
    }

    RendererAPI::~RendererAPI()
    {
        m_NewFrameListener.StopListening();
    }
}
