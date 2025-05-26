//
// Created by Andrew Fagan on 12/4/24.
//

#pragma once


#include "../SceneRenderer.h"
#include "RendererCommands.h"
#include "Resources/Device.h"

namespace Astral {

    class RendererAPI
    {
    public:
        static Device& GetDevice();
        static void Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle);
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawElementsIndexed(CommandBufferHandle commandBuffer, IndexBufferHandle indexBuffer);
        static void SetBlending(bool enable);

    private:

        friend SceneRenderer;
        static RendererCommands* s_RendererCommands;
    };

}
