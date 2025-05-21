//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include <complex.h>

#include "CommandBuffer.h"
#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual void Bind(CommandBufferHandle commandBufferHandle) = 0;
        virtual void Unbind() const {};

        [[nodiscard]] virtual uint32 GetCount() const { return 0; };

        static IndexBuffer* CreateIndexBuffer(uint32* data, uint32 count);
    };

    using IndexBufferHandle = GraphicsRef<IndexBuffer>;

}
