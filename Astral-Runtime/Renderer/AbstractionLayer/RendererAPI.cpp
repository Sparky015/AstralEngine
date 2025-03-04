/**
* @file RendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "RendererAPI.h"

namespace Graphics {

    API RendererAPI::s_RendererAPI = API::OpenGL;

    RendererAPI::RendererAPI()
    {
        m_NewFrameListener.StartListening();
    }

    RendererAPI::~RendererAPI()
    {
        m_NewFrameListener.StopListening();
    }
}
