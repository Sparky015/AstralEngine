//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once


namespace Renderer {

    class OpenGLBufferLayout
    {
    public:
        explicit OpenGLBufferLayout();

        void EnableAttribute();

    private:
        int m_AttributeIndex;
        int m_Size;
        int m_Stride;

        void SetBufferLayout();
    };

}