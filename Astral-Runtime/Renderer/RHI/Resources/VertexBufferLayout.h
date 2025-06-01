//
// Created by Andrew Fagan on 12/2/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <vector>
#include <string>

namespace Astral {

    enum ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4
    };


    struct VertexBufferAttribute
    {
        VertexBufferAttribute(ShaderDataType shaderDataType, std::string_view name) : DataType(shaderDataType) {};
        ShaderDataType DataType;
        uint32 GetAttributeSize() const;
        uint32 GetAttributeComponentCount() const;
    };


    class VertexBufferLayout
    {
    public:
        VertexBufferLayout(std::initializer_list<VertexBufferAttribute> bufferAttributes);

        uint32 GetStride() { return m_Stride; }

        std::vector<VertexBufferAttribute>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexBufferAttribute>::iterator end() { return m_Attributes.end(); }
    private:

        uint32 m_Stride;
        std::vector<VertexBufferAttribute> m_Attributes;
    };

}