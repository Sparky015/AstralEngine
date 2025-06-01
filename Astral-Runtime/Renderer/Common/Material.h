/**
* @file Material.h
* @author Andrew Fagan
* @date 5/24/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    struct Material
    {
        ShaderHandle VertexShader;
        ShaderHandle PixelShader;
        TextureHandle TextureUniform;
    };

}
