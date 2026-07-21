/**
* @file MetalRendererContext.mm
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalRendererContext.h"

#include "Core/Utilities/Asserts.h"
#include "Resources/MetalDevice.h"
#include "Resources/MetalCommandQueue.h"
#include "Renderer/RHI/RendererAPI.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <QuartzCore/CAMetalLayer.h>

namespace Astral {

    MetalRenderingContext::MetalRenderingContext(GLFWwindow* window) :
        m_Window(window),
        m_NewFrameListener(
        {
            EventListener<NewFrameEvent>{[this](NewFrameEvent e){ this->DrainFrameAutoreleasePool(); }}
        })
    {
        m_FrameAutoreleasePool = NS::AutoreleasePool::alloc()->init();
        m_NewFrameListener.StartListening();
    }


    void MetalRenderingContext::Init()
    {
        AllocateCAMetalLayer();
        CreateDevice();
        AttachCALayerToWindow();
        CreateGlobalResidencySet();
        CreatePrimaryCommandQueue();
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
        DestroyPrimaryCommandQueue();
        ReleaseGlobalResidencySet();
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
        MTL::Device* device = (MTL::Device*)m_Device->GetNativeHandle();
        ImGui_ImplMetal_Init(device);



        // ==== Populating ImGui render pass attachment formats struct ========================================================

        m_ImGuiRenderPassAttachmentFormats = AttachmentFormats{};
        m_ImGuiRenderPassAttachmentFormats.SampleCount = 1;

        const std::vector<AttachmentReference>& colorAttachmentReferences = renderPassHandle->GetColorAttachmentReferences();

        for (size_t i = 0; i < colorAttachmentReferences.size(); i++)
        {
            const AttachmentReference& colorAttachmentReference = colorAttachmentReferences[i];
            AttachmentDescription colorAttachmentDescription = renderPassHandle->GetAttachmentDescription(colorAttachmentReference.AttachmentIndex);
            m_ImGuiRenderPassAttachmentFormats.ColorPixelFormat = ConvertImageFormatToMTLPixelFormat(colorAttachmentDescription.Format);
            break;
        }


        AttachmentReference depthStencilAttachmentReference = renderPassHandle->GetDepthStencilAttachmentReference();

        if (depthStencilAttachmentReference.AttachmentIndex != NullAttachmentIndex)
        {
            // Depth stencil attachment exists
            AttachmentDescription depthStencilAttachmentDescription = renderPassHandle->GetAttachmentDescription(depthStencilAttachmentReference.AttachmentIndex);

            m_ImGuiRenderPassAttachmentFormats.DepthPixelFormat = ConvertImageFormatToMTLPixelFormat(depthStencilAttachmentDescription.Format);
            if (IsStencilFormat(depthStencilAttachmentDescription.Format))
            {
                m_ImGuiRenderPassAttachmentFormats.StencilPixelFormat = ConvertImageFormatToMTLPixelFormat(depthStencilAttachmentDescription.Format);
            }
            else
            {
                m_ImGuiRenderPassAttachmentFormats.StencilPixelFormat = MTL::PixelFormatInvalid;
            }
        }

    }


    void MetalRenderingContext::MarkNewImGuiFrame()
    {
        ImGui_ImplMetal_NewFrame(m_ImGuiRenderPassAttachmentFormats);
    }


    void MetalRenderingContext::ShutdownImGuiForAPIBackend()
    {
        ImGui_ImplMetal_Shutdown();
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
        UVec2 windowExtent = RendererAPI::GetContext().GetWindowFramebufferDimensions();

        MTL::Device* metalDevice = (MTL::Device*)m_Device->GetNativeHandle();
        NSWindow* nsWindow = glfwGetCocoaWindow(m_Window);

        CAMetalLayer* caMetalLayer = (__bridge CAMetalLayer*)m_CAMetalLayer;
        caMetalLayer.device = (__bridge id<MTLDevice>)metalDevice;
        caMetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        caMetalLayer.drawableSize = NSMakeSize(windowExtent.x , windowExtent.y);
        caMetalLayer.opaque = YES;
        caMetalLayer.framebufferOnly = YES;
        caMetalLayer.contentsScale = nsWindow.backingScaleFactor;

        nsWindow.contentView.layer = caMetalLayer;
        nsWindow.contentView.wantsLayer = YES;

        NSView* nsView = nsWindow.contentView;
        nsView.layer = caMetalLayer;
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
            m_PrimaryCommandQueue->WaitIdle();
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


    MTL4::CommandAllocator* MetalRenderingContext::AcquireThreadCommandAllocator()
    {
        std::lock_guard lock(m_CommandAllocatorsMutex); // Lock in case of a thread adding new command allocator

        std::thread::id executingThreadID = std::this_thread::get_id();
        if (!m_CommandAllocators.contains(executingThreadID))
        {
            // Create and populate a new command allocator pool for this thread
            CommandAllocatorPool commandAllocatorPool{};
            MTL::Device* device = (MTL::Device*)m_Device->GetNativeHandle();

            for (size_t i = 0; i < 5; i++)
            {
                MTL4::CommandAllocator* newCommandAllocator = device->newCommandAllocator();
                m_CommandAllocatorOwnedThread[newCommandAllocator] = executingThreadID;
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
            MTL::Device* device = (MTL::Device*)m_Device->GetNativeHandle();
            MTL4::CommandAllocator* newCommandAllocator = device->newCommandAllocator();
            m_CommandAllocatorOwnedThread[newCommandAllocator] = executingThreadID;
            commandAllocatorPool.UsedCommandAllocators.insert(newCommandAllocator);
            return newCommandAllocator;
        }
    }


    void MetalRenderingContext::ReleaseThreadCommandAllocator(MTL4::CommandAllocator* commandAllocator)
    {
        std::lock_guard lock(m_CommandAllocatorsMutex); // Lock in case of a thread adding new command allocator

        if (!m_CommandAllocatorOwnedThread.contains(commandAllocator))
        {
            AE_WARN("[MetalRenderingContext] Given command allocator is not tracked by the metal renderer context!")
            return;
        }

        std::thread::id ownedThreadID = m_CommandAllocatorOwnedThread.at(commandAllocator);
        if (!m_CommandAllocators.contains(ownedThreadID))
        {
            AE_WARN("[MetalRenderingContext] Given command allocator is not from this thread's command allocator pool!")
            return;
        }

        CommandAllocatorPool& allocatorPool = m_CommandAllocators.at(ownedThreadID);

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


    MTL::ResidencySet* MetalRenderingContext::GetGlobalResidencySet()
    {
        return m_GlobalResidencySet;
    }


    std::mutex& MetalRenderingContext::GetGlobalResidencySetMutex()
    {
        return m_GlobalResidencySetMutex;
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


    inline void MetalRenderingContext::CreateGlobalResidencySet()
    {
        MTL::Device* mtlDevice = (MTL::Device*)m_Device->GetNativeHandle();
        NS::Error* error = nullptr;
        MTL::ResidencySetDescriptor* residencySetDescriptor = MTL::ResidencySetDescriptor::alloc()->init();
        m_GlobalResidencySet = mtlDevice->newResidencySet(residencySetDescriptor, &error);
        residencySetDescriptor->release();

        if (m_GlobalResidencySet == nullptr && error)
        {
            AE_ERROR("Residency set failed to be created! Error: " << error->localizedDescription()->utf8String())
            error->release();
        }

        m_GlobalResidencySet->requestResidency();
    }


    inline void MetalRenderingContext::ReleaseGlobalResidencySet()
    {
        if (m_GlobalResidencySet)
        {
            m_GlobalResidencySet->release();
            m_GlobalResidencySet = nullptr;
        }
    }


    void MetalRenderingContext::DrainFrameAutoreleasePool()
    {
        if (m_FrameAutoreleasePool)
        {
            m_FrameAutoreleasePool->drain();
            m_FrameAutoreleasePool = nullptr;
        }

        m_FrameAutoreleasePool = NS::AutoreleasePool::alloc()->init();
    }

}
