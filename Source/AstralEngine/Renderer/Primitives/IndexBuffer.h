//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

namespace Graphics {

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        [[nodiscard]] virtual uint32 GetCount() const = 0;

        static IndexBuffer* CreateIndexBuffer(uint32* data, uint32 count);
    };

} // Renderer
