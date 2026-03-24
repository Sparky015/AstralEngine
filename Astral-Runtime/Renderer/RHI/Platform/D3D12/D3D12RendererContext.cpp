/**
* @file D3D12RendererContext.cpp
* @author Andrew Fagan
* @date 3/24/2026
*/

#include "D3D12RendererContext.h"

#include "Debug/Utilities/Asserts.h"
#include "Resources/D3D12Device.h"


namespace Astral {

    D3D12RenderingContext::D3D12RenderingContext(GLFWwindow* window) :
        m_Window(window)
    {

    }


    void D3D12RenderingContext::Init()
    {
#ifndef ASTRAL_DISABLE_RENDERING_API_VALIDATION
        EnableDebugLayer();
#endif
        CreateDXGIFactory();
        CreateDevice();
    }


    void D3D12RenderingContext::Shutdown()
    {
        DestroyDevice();
        DestroyDXGIFactory();
    }


    Device& D3D12RenderingContext::GetDevice()
    {
        ASSERT(m_Device, "Cannot retrieve device instance before it is created. Device handle is null!")
        return *m_Device;
    }


    uint32 D3D12RenderingContext::GetNumValidationErrorsAndWarnings()
    {
        return 0;
    }


    void D3D12RenderingContext::ClearNumValidationErrorsAndWarnings()
    {

    }


    void D3D12RenderingContext::InitImGuiForAPIBackend(RenderPassHandle renderPassHandle)
    {

    }


    void D3D12RenderingContext::ShutdownImGuiForAPIBackend()
    {

    }


    void D3D12RenderingContext::EnableDebugLayer()
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
    }


    void D3D12RenderingContext::CreateDXGIFactory()
    {
        UINT dxgiFactoryFlags = 0;

#ifndef ASTRAL_DISABLE_RENDERING_API_VALIDATION
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

        HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_Factory));
        ASSERT(!FAILED(result), "DXGI Factory failed to create!")
    }

    void D3D12RenderingContext::DestroyDXGIFactory()
    {
        m_Factory.Reset();
    }


    void D3D12RenderingContext::CreateDevice()
    {
        D3D12DeviceDesc d3d12DeviceDesc = {

        };

        m_Device = CreateGraphicsOwnedPtr<D3D12Device>(d3d12DeviceDesc);
    }


    void D3D12RenderingContext::DestroyDevice()
    {
        m_Device.reset();
    }

}
