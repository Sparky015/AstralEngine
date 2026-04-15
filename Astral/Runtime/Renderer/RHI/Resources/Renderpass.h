/**
* @file Renderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Framebuffer.h"
#include "Renderer/RHI/Common/AccessFlags.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/PipelineStageFlags.h"
#include "Renderer/RHI/Common/SampleCount.h"


namespace Astral {

    enum class AttachmentLoadOp : uint8
    {
        LOAD,
        CLEAR,
        DONT_CARE,
        NONE
    };

    enum class AttachmentStoreOp : uint8
    {
        STORE,
        DONT_CARE,
        NONE
    };

    struct AttachmentDescription
    {
        ImageFormat Format;
        ImageUsageFlags ImageUsageFlags;
        AttachmentLoadOp LoadOp;
        AttachmentStoreOp StoreOp;
        ImageLayout InitialLayout;
        ImageLayout FinalLayout;
        Vec4 ClearColor; // Only use when load op is set to clear
        SampleCount MSAASamples = SampleCount::SAMPLE_1_BIT;
        uint32 LayerCount = 1;
        uint32 MipMapCount = 1;
        TextureType TextureType = TextureType::IMAGE_2D;

        bool operator==(const AttachmentDescription&) const = default;
    };

    struct SubpassDependencyMasks
    {
        PipelineStageFlags SourceStageMask;
        PipelineStageFlags DestinationStageMask;
        AccessFlags SourceAccessMask;
        AccessFlags DestinationAccessMask;
    };

    using AttachmentIndex = uint32;
    static constexpr AttachmentIndex NullAttachmentIndex = -1;
    using SubpassIndex = uint8;
    static constexpr SubpassIndex NullSubpassIndex = -1;
    static constexpr SubpassIndex SubpassExternal = -1;

    /**
     * @brief Defines the RHI RenderPass object
     * @warning The current implementation only supports single threaded recording
     */
    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void BeginBuildingRenderPass() = 0;
        virtual AttachmentIndex DefineAttachment(const AttachmentDescription& attachmentDescription) = 0;
        virtual void BeginBuildingSubpass() = 0;
        virtual void AddInputAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;
        virtual void AddColorAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;
        virtual void AddResolveAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;
        virtual void AddDepthStencilAttachment(AttachmentIndex attachmentIndex, ImageLayout optimalImageLayout) = 0;
        virtual void PreserveAttachment(AttachmentIndex attachmentIndex) = 0;
        virtual SubpassIndex EndBuildingSubpass() = 0;
        virtual void DefineSubpassDependency(SubpassIndex sourceSubpass, SubpassIndex destinationSubpass, SubpassDependencyMasks subpassDependencyMasks) = 0;
        virtual void EndBuildingRenderPass() = 0;

        virtual void Invalidate() = 0;

        virtual void BeginRenderPass(FramebufferHandle frameBufferHandle) = 0;
        virtual void NextSubpass() = 0;
        virtual void EndRenderPass() = 0;

        virtual uint32 GetNumberOfSubpasses() = 0;
        virtual uint32 GetNumColorAttachments(SubpassIndex subpassIndex) = 0;
        virtual const std::vector<Vec4>& GetClearColors() const = 0;

        virtual void* GetNativeHandle() = 0;
    };

    using RenderPassHandle = GraphicsRef<RenderPass>;

}
