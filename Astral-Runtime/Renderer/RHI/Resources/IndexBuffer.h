//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        [[nodiscard]] virtual uint32 GetCount() const = 0;

        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<IndexBuffer> CreateIndexBuffer(uint32* data, uint32 count);
    };

    using IndexBufferHandle = GraphicsRef<IndexBuffer>;

}
