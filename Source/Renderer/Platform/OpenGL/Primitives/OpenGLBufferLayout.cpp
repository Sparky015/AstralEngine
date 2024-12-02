//
// Created by Andrew Fagan on 7/24/24.
//

#include "OpenGLBufferLayout.h"

namespace Renderer {


    uint32_t BufferAttributes::GetElementCount()
    {
        switch (DataType)
        {
            case Float4: return 4;
        }
    }


    Renderer::OpenGLBufferLayout::OpenGLBufferLayout(std::initializer_list<BufferAttributes> bufferAttributes) : m_Attributes(bufferAttributes)
    {

    }

}