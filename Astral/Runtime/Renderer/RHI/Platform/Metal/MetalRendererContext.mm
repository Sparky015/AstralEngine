/**
* @file MetalRendererContext.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalRendererContext.h"

#include "Core/Utilities/Asserts.h"
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
        m_PipelineStateCache = CreateGraphicsOwnedPtr<PipelineStateCache>();
        CreatePipelineDataSetSerializer();
        CreateCompiler();
    }


    void MetalRenderingContext::Shutdown()
    {
        ReleaseAllCommandAllocatorPools();
        ReleaseCompiler();
        ReleasePipelineDataSetSerializer();
        m_PipelineStateCache.reset();
        DestroyDevice();
        ReleaseCAMetalLayer();
    }


    Device& MetalRenderingContext::GetDevice()
    {
        ASSERT(m_Device, "Metal Device has not been created! Cannot retrieve null device!");
        return *m_Device;
    }


    PipelineStateCache& MetalRenderingContext::GetPipelineStateCache()
    {
        return *m_PipelineStateCache;
    }


    uint32 MetalRenderingContext::GetNumValidationErrorsAndWarnings() { return 0; }


    void MetalRenderingContext::ClearNumValidationErrorsAndWarnings() {}


    void MetalRenderingContext::InitImGuiForAPIBackend(RenderPassHandle renderPassHandle)
    {

    }


    void MetalRenderingContext::ShutdownImGuiForAPIBackend()
    {

    }


    CommandQueueHandle MetalRenderingContext::GetPrimaryCommandQueue()
    {
        return m_PrimaryCommandQueue;
    }


    MTL4::Compiler* MetalRenderingContext::GetCompiler()
    {
        return m_Compiler;
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
        if (m_CAMetalLayer)
        {
            m_CAMetalLayer->release();
        }
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


    void MetalRenderingContext::CreatePipelineDataSetSerializer()
    {
        MTL::Device* mtlDevice = (MTL::Device*)m_Device->GetNativeHandle();
        MTL4::PipelineDataSetSerializerDescriptor* pipelineDataSetSerializerDescriptor = MTL4::PipelineDataSetSerializerDescriptor::alloc()->init();
        m_PipelineDataSetSerializer = mtlDevice->newPipelineDataSetSerializer(pipelineDataSetSerializerDescriptor);
        pipelineDataSetSerializerDescriptor->release();
    }


    void MetalRenderingContext::ReleasePipelineDataSetSerializer()
    {
        if (m_PipelineDataSetSerializer)
        {
            m_PipelineDataSetSerializer->release();
        }
    }


    void MetalRenderingContext::CreatePrimaryCommandQueue()
    {
        MTL::Device* device = (MTL::Device*)m_Device->GetNativeHandle();

        MetalCommandQueueDesc commandQueueDesc = {
            .Device = device
        };

        m_PrimaryCommandQueue = CreateGraphicsRef<MetalCommandQueue>(commandQueueDesc);
    }


    void MetalRenderingContext::DestroyPrimaryCommandQueue()
    {
        if (m_PrimaryCommandQueue)
        {
            m_PrimaryCommandQueue.reset();
            m_PrimaryCommandQueue = nullptr;
        }
    }


    void MetalRenderingContext::CreateCompiler()
    {
        MTL::Device* mtlDevice = (MTL::Device*)m_Device->GetNativeHandle();
        NS::Error* error = nullptr;
        MTL4::CompilerDescriptor* compilerDescriptor = MTL4::CompilerDescriptor::alloc()->init();
        compilerDescriptor->setPipelineDataSetSerializer(m_PipelineDataSetSerializer);
        m_Compiler = mtlDevice->newCompiler(compilerDescriptor, &error);

        if (error)
        {
            AE_ERROR("Metal compiler failed to be created! " << error->localizedDescription()->utf8String());
            error->release();
        }

        compilerDescriptor->release();
    }


    void MetalRenderingContext::ReleaseCompiler()
    {
        if (m_Compiler)
        {
            m_Compiler->release();
            m_Compiler = nullptr;
        }
    }


    void MetalRenderingContext::ReleaseThreadCommandAllocator(MTL4::CommandAllocator* commandAllocator)
    {
        std::lock_guard lock(m_CommandAllocatorsMutex); // Lock in case of a thread adding new command allocator

        std::thread::id executingThreadID = std::this_thread::get_id();
        if (!m_CommandAllocators.contains(executingThreadID))
        {
            AE_WARN("[MetalRenderingContext] Given command allocator is not from this thread's command allocator pool!")
            return;
        }

        CommandAllocatorPool& allocatorPool = m_CommandAllocators.at(executingThreadID);

        if (allocatorPool.UsedCommandAllocators.contains(commandAllocator))
        {
            allocatorPool.UsedCommandAllocators.erase(commandAllocator);
            allocatorPool.AvailableCommandAllocators.insert(commandAllocator);
        }
        else
        {
            AE_WARN("[MetalRenderingContext] Given command allocator was not acquired from this thread's command allocator pool!")
        }
    }

    MTL4::CommandAllocator* MetalRenderingContext::AcquireThreadCommandAllocator()
    {
        std::lock_guard lock(m_CommandAllocatorsMutex); // Lock in case of a thread adding new command allocator

        std::thread::id executingThreadID = std::this_thread::get_id();
        if (!m_CommandAllocators.contains(executingThreadID))
        {
            // Create and populate a new command allocator pool for this thread
            CommandAllocatorPool commandAllocatorPool{};
            MTL::Device* device = (MTL::Device*)m_Device->GetNativeHandle();

            for (size_t i = 0; i < 3; i++)
            {
                MTL4::CommandAllocator* newCommandAllocator = device->newCommandAllocator();
                commandAllocatorPool.AvailableCommandAllocators.insert(newCommandAllocator);
            }
            m_CommandAllocators.emplace(executingThreadID, commandAllocatorPool);
        }

        // Try to acquire a command allocator from this thread's command pool
        CommandAllocatorPool& commandAllocatorPool = m_CommandAllocators.at(executingThreadID);

        if (commandAllocatorPool.AvailableCommandAllocators.size() != 0)
        {
            MTL4::CommandAllocator* acquiredCommandAllocator = *commandAllocatorPool.AvailableCommandAllocators.begin();
            commandAllocatorPool.AvailableCommandAllocators.erase(acquiredCommandAllocator);
            commandAllocatorPool.UsedCommandAllocators.insert(acquiredCommandAllocator);
            return acquiredCommandAllocator;
        }
        else
        {
            AE_WARN("[MetalRenderingContext] This thread's command allocator pool is all used! Can't acquire a command allocator!")
            return nullptr;
        }
    }


    void MetalRenderingContext::ReleaseAllCommandAllocatorPools()
    {
        for (auto& [threadID, commandAllocatorPool] : m_CommandAllocators)
        {
            for (MTL4::CommandAllocator* commandAllocator : commandAllocatorPool.AvailableCommandAllocators)
            {
                commandAllocator->release();
            }
            for (MTL4::CommandAllocator* commandAllocator : commandAllocatorPool.UsedCommandAllocators)
            {
                commandAllocator->release();
            }
        }
    }

}
