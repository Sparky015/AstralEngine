//
// Created by Andrew Fagan on 12/2/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Core/Utility/Hashing/HashCombiner.h"

#include <vector>
#include <string>


namespace Astral {

    enum ShaderDataType : uint8
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

        bool operator==(const VertexBufferAttribute& other) const
        {
            return DataType == other.DataType;
        }
    };


    class VertexBufferLayout
    {
    public:
        VertexBufferLayout(std::initializer_list<VertexBufferAttribute> bufferAttributes);

        void AddAttribute(VertexBufferAttribute attribute)
        {
            m_Attributes.push_back(attribute);
            CalcStride();
        }

        uint32 GetStride() const { return m_Stride; }

        const std::vector<VertexBufferAttribute>& GetAttributes() const { return m_Attributes; }

        void CopyAttributes(const VertexBufferLayout& otherBufferLayout)
        {
            m_Attributes.assign(otherBufferLayout.m_Attributes.cbegin(), otherBufferLayout.m_Attributes.cend());
        }

        std::vector<VertexBufferAttribute>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexBufferAttribute>::iterator end() { return m_Attributes.end(); }

        bool operator==(const VertexBufferLayout& other) const
        {
            return m_Attributes == other.m_Attributes;
        }

    private:

        void CalcStride();

        uint32 m_Stride;
        std::vector<VertexBufferAttribute> m_Attributes;
    };

}