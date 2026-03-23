/**
* @file MetalRendererContext.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalRendererContext.h"

#include "Debug/Utilities/Asserts.h"
#include "Resources/MetalDevice.h"

namespace Astral {

    MetalRenderingContext::MetalRenderingContext(GLFWwindow* window) :
        m_Window(window)
    {

    }


    void MetalRenderingContext::Init()
    {
        CreateDevice();
    }


    void MetalRenderingContext::Shutdown()
    {
        DestroyDevice();
    }


    Device& MetalRenderingContext::GetDevice()
    {
        ASSERT(m_Device, "Metal Device has not been created! Cannot retrieve null device!");
        return *m_Device;
    }


    uint32 MetalRenderingContext::GetNumValidationErrorsAndWarnings()
    {
        return 0;
    }


    void MetalRenderingContext::ClearNumValidationErrorsAndWarnings()
    {
    }


    void MetalRenderingContext::InitImGuiForAPIBackend(RenderPassHandle renderPassHandle)
    {

    }


    void MetalRenderingContext::ShutdownImGuiForAPIBackend()
    {

    }


    void MetalRenderingContext::CreateDevice()
    {
        MetalDeviceDesc metalDeviceDesc = {

        };

        m_Device = CreateGraphicsOwnedPtr<MetalDevice>(metalDeviceDesc);
        m_Device->Init();
    }


    void MetalRenderingContext::DestroyDevice()
    {
        m_Device.release();
    }

}
