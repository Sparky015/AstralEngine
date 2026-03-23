/**
* @file MetalRendererContext.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalRendererContext.h"

#include "Debug/Utilities/Asserts.h"
#include "Resources/MetalDevice.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <QuartzCore/CAMetalLayer.h>

namespace Astral {

    MetalRenderingContext::MetalRenderingContext(GLFWwindow* window) :
        m_Window(window)
    {

    }


    void MetalRenderingContext::Init()
    {
        AllocateCAMetalLayer();
        CreateDevice();
        AttachCALayerToWindow();
    }


    void MetalRenderingContext::Shutdown()
    {
        DestroyDevice();
        ReleaseCAMetalLayer();
    }


    Device& MetalRenderingContext::GetDevice()
    {
        ASSERT(m_Device, "Metal Device has not been created! Cannot retrieve null device!");
        return *m_Device;
    }


    uint32 MetalRenderingContext::GetNumValidationErrorsAndWarnings() { return 0; }


    void MetalRenderingContext::ClearNumValidationErrorsAndWarnings() {}


    void MetalRenderingContext::InitImGuiForAPIBackend(RenderPassHandle renderPassHandle)
    {

    }


    void MetalRenderingContext::ShutdownImGuiForAPIBackend()
    {

    }


    void MetalRenderingContext::CreateDevice()
    {
        MetalDeviceDesc metalDeviceDesc = {
            .CAMetalLayer = m_CAMetalLayer
        };

        m_Device = CreateGraphicsOwnedPtr<MetalDevice>(metalDeviceDesc);
        m_Device->Init();
    }


    void MetalRenderingContext::DestroyDevice()
    {
        m_Device.reset();
    }


    void MetalRenderingContext::AllocateCAMetalLayer()
    {
        m_CAMetalLayer = CA::MetalLayer::layer()->retain(); // Allocates a CAMetalLayer
    }


    void MetalRenderingContext::ReleaseCAMetalLayer()
    {
        if (m_CAMetalLayer) { m_CAMetalLayer->release(); }
    }


    void MetalRenderingContext::AttachCALayerToWindow()
    {
        MTL::Device* metalDevice = (MTL::Device*)m_Device->GetNativeHandle();
        NSWindow* nsWindow = glfwGetCocoaWindow(m_Window);

        CAMetalLayer* caMetalLayer = (__bridge CAMetalLayer*)m_CAMetalLayer;
        caMetalLayer.device = (__bridge id<MTLDevice>)metalDevice;
        caMetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;

        nsWindow.contentView.layer = caMetalLayer;
        nsWindow.contentView.wantsLayer = YES;
    }

}
