/**
* @file OpenGLRendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "OpenGLRendererCommands.h"

#include "glad/glad.h"

namespace Astral {

    void OpenGLRendererCommands::Clear(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget)
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void OpenGLRendererCommands::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }


    void OpenGLRendererCommands::DrawElementsIndexed(CommandBufferHandle commandBufferHandle, IndexBufferHandle vertexBufferHandle)
    {
        m_NumberOfDrawCalls++;
        // commandBufferHandle->Bind();
        // glDrawElements(GL_TRIANGLES, commandBufferHandle->GetElementCount(), GL_UNSIGNED_INT, nullptr);
    }


    void OpenGLRendererCommands::SetBlending(bool enable)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

    }

}
