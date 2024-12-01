//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

namespace Renderer {

#define GLCheckError() while (GLenum error = glGetError()) { WARN("[OpenGL] Error Code: " << error); }


    enum class RendererAPI : uint8
    {
        OpenGL = 0, Vulkan, DirectX12, Metal
    };

    class Renderer
    {
    public:
        static inline RendererAPI GetAPI() {return s_RendererAPI;}

    private:
        static RendererAPI s_RendererAPI;
    };

} // Renderer
