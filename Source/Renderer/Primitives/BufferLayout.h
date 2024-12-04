//
// Created by Andrew Fagan on 12/2/24.
//

#pragma once

namespace Renderer {

    enum ShaderDataTypes
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


    struct BufferAttribute
    {
        explicit BufferAttribute(ShaderDataTypes shaderDataTypes, std::string&& name) : DataType(shaderDataTypes) {};
        ShaderDataTypes DataType;
        uint32 GetAttributeSize() const;
        uint32 GetAttributeComponentCount() const;
    };


    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferAttribute> bufferAttributes);



        std::vector<BufferAttribute>::iterator begin() { return m_Attributes.begin(); }
        std::vector<BufferAttribute>::iterator end() { return m_Attributes.end(); }
    private:

        std::vector<BufferAttribute> m_Attributes;
    };

}