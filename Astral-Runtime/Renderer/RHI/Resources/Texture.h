//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

#include <string>

namespace Astral {

    class Texture
    {
    public:
        virtual ~Texture() = default;
        virtual void Bind(unsigned int slot = 0) {};
        virtual void Unbind() {};
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;

        virtual void* GetSampler() = 0;
        virtual void* GetNativeHandle() = 0;

        static Texture* CreateTexture(const std::string& filePath);
    };

    using TextureHandle = GraphicsRef<Texture>;

} // Renderer
