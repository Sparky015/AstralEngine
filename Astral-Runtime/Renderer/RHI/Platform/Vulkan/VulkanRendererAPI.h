/**
* @file VulkanRendererAPI.h
* @author Andrew Fagan
* @date 3/4/25
*/

#pragma once

#include "Renderer/RHI/RendererAPI.h"

namespace Graphics {

class VulkanRendererAPI : public RendererAPI
{
    void Clear() override;
    void SetClearColor(float r, float g, float b, float a) override;
    void DrawElements(VertexArrayObject* vertexArrayObject) override;
    void SetBlending(bool enable) override;
};

}
