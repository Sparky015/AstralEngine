//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once

#include "Renderer/Primitives/BufferLayout.h"

namespace Renderer {

    struct BufferAttributes
    {
        ShaderDataTypes DataType;
        uint32 GetElementCount();
    };


    class OpenGLBufferLayout
    {
    public:
        explicit OpenGLBufferLayout(std::initializer_list<BufferAttributes> bufferAttributes);


//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // slot, number of elements, something, something, size of stride (total size of each element), offset
    private:

        std::vector<BufferAttributes> m_Attributes;
    };

}