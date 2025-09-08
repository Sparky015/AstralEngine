//
// Created by Andrew Fagan on 12/2/24.
//

#include "VertexBufferLayout.h"

#include "Debug/Utilities/Error.h"

namespace Astral {

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferAttribute> bufferAttributes) :
        m_Stride(0),
        m_Attributes(bufferAttributes)
    {
        CalcStride();
    }


    void VertexBufferLayout::CalcStride()
    {
        m_Stride = 0;
        for (VertexBufferAttribute& attribute : m_Attributes)
        {
            m_Stride += attribute.GetAttributeSize();
        }
    }


    uint32_t VertexBufferAttribute::GetAttributeSize() const
    {
        switch (DataType)
        {
            case Float:  return sizeof(float) * 1;
            case Float2: return sizeof(float) * 2;
            case Float3: return sizeof(float) * 3;
            case Float4: return sizeof(float) * 4;
            case Int:    return sizeof(int) * 1;
            case Int2:   return sizeof(int) * 2;
            case Int3:   return sizeof(int) * 3;
            case Int4:   return sizeof(int) * 4;
            default: AE_ERROR("Invalid Attribute Type");
        }
    }

    uint32_t VertexBufferAttribute::GetAttributeComponentCount() const
    {
        switch (DataType)
        {
            case Float:   return 1;
            case Float2:  return 2;
            case Float3:  return 3;
            case Float4:  return 4;
            case Int:     return 1;
            case Int2:    return 2;
            case Int3:    return 3;
            case Int4:    return 4;
            default: AE_ERROR("Invalid Attribute Type");
        }
    }

}
