// dear imgui: Renderer Backend for Metal 4
// This needs to be used along with a Platform Backend (e.g. OSX)
// Metal 4 requires Apple Silicon and macOS 26+.

// Implemented features:
//  [X] Renderer: User texture binding. Use 'MTLTexture' as texture identifier. Read the FAQ about ImTextureID/ImTextureRef!
//  [X] Renderer: Large meshes support (64k+ vertices) even with 16-bit indices (ImGuiBackendFlags_RendererHasVtxOffset).
//  [X] Renderer: Texture updates support for dynamic font atlas (ImGuiBackendFlags_RendererHasTextures).
//  [X] Renderer: Multi-viewport support (multiple windows).

// Missing features or Issues:
//  [ ] Texture view pool support? Reevaluate which type to use for ImtextureID.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2026-07-07: Metal 4: Added metal-cpp support. (#9461)
//  2026-07-02: Metal 4: Added new Metal 4 backend implementation. (#9458)

#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_impl_metal4.h"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#if TARGET_OS_OSX
#import <AppKit/AppKit.h>
#endif

#ifdef IMGUI_IMPL_METAL_CPP
#include <Metal/Metal.hpp>
#endif

// Forward Declarations
static void ImGui_ImplMetal_InitMultiViewportSupport();
static void ImGui_ImplMetal_ShutdownMultiViewportSupport();
static void ImGui_ImplMetal_CreateDeviceObjectsForPlatformWindows();
static void ImGui_ImplMetal_InvalidateDeviceObjectsForPlatformWindows();

#pragma mark - Support classes and structs

#define METAL_IMGUI_VIEWPORTS_PER_CHUNK 64
// Metal requires constant [[buffer]] offsets to be 256-byte aligned (Metal3 ImGui advances by 256).
#define METAL_IMGUI_CONSTANT_ALIGN 256

struct ImGui_Metal4_ConstantSlot
{
    float ModelViewProjectionMatrix[4][4]; // 64 bytes
    uint8_t _pad[METAL_IMGUI_CONSTANT_ALIGN - sizeof(float) * 16];
};
static_assert(sizeof(ImGui_Metal4_ConstantSlot) == METAL_IMGUI_CONSTANT_ALIGN, "Constant slot must be 256-byte aligned");

struct ImGui_Metal4_ConstantData
{
    // Up to 64 viewports share one constant buffer; each slot is 256-byte aligned for Metal constant fetches.
    ImGui_Metal4_ConstantSlot slots[METAL_IMGUI_VIEWPORTS_PER_CHUNK];
};

// One FIF slot of growable Shared VB/IB. Metal 4 CBs do not retain resources — never overwrite a slot still readable by in-flight GPU work.
@interface ImGuiMetal4FrameRenderBuffers : NSObject
@property (nonatomic, strong) id<MTLBuffer> vertexBuffer;
@property (nonatomic, strong) id<MTLBuffer> indexBuffer;
@property (nonatomic, assign) NSUInteger    vertexBufferSize;
@property (nonatomic, assign) NSUInteger    indexBufferSize;
@end

// Per-viewport FIF-sized VB/IB ring: slot == backend currentFrameSlot (advanced in NewFrame).
@interface ImGuiMetal4WindowRenderBuffers : NSObject
@property (nonatomic, assign) NSUInteger    index; // unused; kept for layout compatibility
@property (nonatomic, strong) NSMutableArray<ImGuiMetal4FrameRenderBuffers*>* frames;
@end

// An object that encapsulates the data necessary to uniquely identify a
// render pipeline state. These are used as cache keys.
@interface FramebufferDescriptor : NSObject<NSCopying>
@property (nonatomic, assign) unsigned long  sampleCount;
@property (nonatomic, assign) MTLPixelFormat colorPixelFormat;
@property (nonatomic, assign) MTLPixelFormat depthPixelFormat;
@property (nonatomic, assign) MTLPixelFormat stencilPixelFormat;
- (instancetype)initWithRenderPassDescriptor:(AttachmentFormats)attachmentFormats;
@end

@interface MetalTexture : NSObject
@property (nonatomic, strong) id<MTLTexture> metalTexture;
- (instancetype)initWithTexture:(id<MTLTexture>)metalTexture;
@end

// A singleton that stores long-lived objects that are needed by the Metal
// renderer backend. Stores the render pipeline state cache and the default
// font texture.
@interface MetalContext : NSObject
@property (nonatomic, strong) id<MTLDevice>                 device;
@property (nonatomic, strong) id<MTL4CommandQueue>          commandQueue;
@property (nonatomic, strong) id<MTLDepthStencilState>      depthStencilState;
// One argument table per command buffer per stage — vertex (VB+constants) and fragment (texture+sampler)
// share no bind space. Metal by Example: separate tables per stage; one shared table put the VB
// GPU address into the fragment buffer(0) slot as well.
@property (nonatomic, strong) NSMapTable<id<MTL4CommandBuffer>, id<MTL4ArgumentTable>>* vertexArgumentTablesForCommandBuffers;
@property (nonatomic, strong) NSMapTable<id<MTL4CommandBuffer>, id<MTL4ArgumentTable>>* fragmentArgumentTablesForCommandBuffers;
@property (nonatomic, strong) id<MTLSamplerState>           samplerStateLinear;
@property (nonatomic, strong) id<MTLSamplerState>           samplerStateNearest;
@property (nonatomic, strong) id<MTLResidencySet>           residencySet;
@property (nonatomic, strong) FramebufferDescriptor*        framebufferDescriptor;
@property (nonatomic, strong) NSMutableDictionary*          renderPipelineStateCache;
@property (nonatomic, assign) NSUInteger                    framesInFlight;
@property (nonatomic, assign) NSUInteger                    currentFrameSlot; // backend-owned; advanced each NewFrame
@property (nonatomic, strong) NSArray<id<MTLSharedEvent>>*  events; // secondary path signals; used to gate command-allocator reset
@property (nonatomic, strong) NSMutableArray<NSNumber*>*     eventValues; // per-slot signaled value (one per frame-in-flight slot)
@property (nonatomic, strong) NSArray<id<MTL4CommandAllocator>>* commandAllocators;
@property (nonatomic, strong) NSMutableArray<NSMutableArray<id<MTLBuffer>>*>*  constantBuffers;
@property (nonatomic) uint64_t                              constantBufferChunkCount;
@property (nonatomic) uint64_t                              currentConstantBufferIndex;
// CoW atlas: old Shared textures stay alive until this backend FIF slot is reused (GPU may still sample them).
@property (nonatomic, strong) NSMutableArray<NSMutableArray<id<MTLTexture>>*>* retiredTextures;
- (void)vertexArgumentTable:(id<MTL4ArgumentTable>*)outVertex
    fragmentArgumentTable:(id<MTL4ArgumentTable>*)outFragment
       forCommandBuffer:(id<MTL4CommandBuffer>)commandBuffer;
- (id<MTLRenderPipelineState>)renderPipelineStateForFramebufferDescriptor:(FramebufferDescriptor*)descriptor device:(id<MTLDevice>)device;
@end

struct ImGui_ImplMetal4_Data
{
    MetalContext*                SharedMetalContext;
    id<MTL4RenderCommandEncoder> RenderCommandEncoder;
    id<MTL4ArgumentTable>        CurrentVertexArgumentTable;
    id<MTL4ArgumentTable>        CurrentFragmentArgumentTable; // sampler/texture callbacks mutate this

    ImGui_ImplMetal4_Data()       { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplMetal4_Data*    ImGui_ImplMetal4_GetBackendData()    { return ImGui::GetCurrentContext() ? (ImGui_ImplMetal4_Data*)ImGui::GetIO().BackendRendererUserData : nullptr; }
static void                      ImGui_ImplMetal4_DestroyBackendData(){ IM_DELETE(ImGui_ImplMetal4_GetBackendData()); }

// Per-viewport renderer data. Main viewport: RenderBuffers only. Secondary: + CAMetalLayer path.
struct ImGuiViewportDataMetal
{
    CAMetalLayer*                      MetalLayer = nullptr;
    id<MTLCommandQueue>                CommandQueue = nil;
    MTL4RenderPassDescriptor*          RenderPassDescriptor = nil;
    id<MTL4CommandBuffer>              CommandBuffer = nil; // reusable Metal 4 CB — one argument table maps to this CB
    void*                              Handle = nullptr;
    bool                               FirstFrame = true;
    ImGuiMetal4WindowRenderBuffers*    RenderBuffers = nil; // Vulkan-style independent VB/IB ring
};

static void ImGui_ImplMetal4_CreateOrResizeBuffer(ImGuiMetal4FrameRenderBuffers* rb, BOOL is_vertex, NSUInteger new_size);
static void ImGui_ImplMetal4_DestroyWindowRenderBuffers(ImGuiMetal4WindowRenderBuffers* wrb);
static ImGuiMetal4WindowRenderBuffers* ImGui_ImplMetal4_EnsureWindowRenderBuffers(ImGuiViewportDataMetal* viewport_data);
static ImGuiMetal4FrameRenderBuffers* ImGui_ImplMetal4_FrameRenderBuffersForSlot(ImGuiMetal4WindowRenderBuffers* wrb, NSUInteger slot);
static id<MTLTexture> ImGui_ImplMetal4_CreateSharedAtlasTexture(ImGui_ImplMetal4_Data* bd, ImTextureData* tex);
static void ImGui_ImplMetal4_RetireTexture(MetalContext* ctx, id<MTLTexture> texture);
static void ImGui_ImplMetal4_FlushRetiredTextures(MetalContext* ctx, NSUInteger slot);
static void ImGui_ImplMetal4_FlushAllRetiredTextures(MetalContext* ctx);

#ifdef IMGUI_IMPL_METAL_CPP

#pragma mark - Dear ImGui Metal C++ Backend API

bool ImGui_ImplMetal4_Init(MTL::Device* device, MTL4::CommandQueue* commandQueue, int framesInFlight)
{
    return ImGui_ImplMetal4_Init((__bridge id<MTLDevice>)(device),(__bridge id<MTL4CommandQueue>)(commandQueue), framesInFlight);
}

void ImGui_ImplMetal4_RenderDrawData(ImDrawData* draw_data,
                                    MTL4::CommandBuffer* commandBuffer,
                                    MTL4::RenderCommandEncoder* commandEncoder)
{
    ImGui_ImplMetal4_RenderDrawData(draw_data,
                                   (__bridge id<MTL4CommandBuffer>)(commandBuffer),
                                   (__bridge id<MTL4RenderCommandEncoder>)(commandEncoder));
}

bool ImGui_ImplMetal_CreateDeviceObjects(MTL::Device* device)
{
    return ImGui_ImplMetal4_CreateDeviceObjects((__bridge id<MTLDevice>)(device));
}

#endif // #ifdef IMGUI_IMPL_METAL_CPP

#pragma mark - Dear ImGui Metal Backend API

void ImGui_ImplMetal4_NewFrame(AttachmentFormats attachmentFormats)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    IM_ASSERT(bd != nil && "Context or backend not initialized! Did you call ImGui_ImplMetal4_Init()?");
    IM_ASSERT(bd->SharedMetalContext.framesInFlight > 0);

    // Independent FIF index — do not require the app to pass its own frame slot.
    bd->SharedMetalContext.currentFrameSlot =
        (bd->SharedMetalContext.currentFrameSlot + 1) % bd->SharedMetalContext.framesInFlight;

    bd->SharedMetalContext.framebufferDescriptor = [[FramebufferDescriptor alloc] initWithRenderPassDescriptor:attachmentFormats];
    if (bd->SharedMetalContext.depthStencilState == nil)
        ImGui_ImplMetal4_CreateDeviceObjects(bd->SharedMetalContext.device);

    bd->SharedMetalContext.currentConstantBufferIndex = 0;
    NSUInteger slot = bd->SharedMetalContext.currentFrameSlot;
    // Slot reuse ⇒ GPU finished the prior occupant of this FIF index — safe to drop CoW-retired atlases queued here.
    ImGui_ImplMetal4_FlushRetiredTextures(bd->SharedMetalContext, slot);
    uint64_t waitValue = [bd->SharedMetalContext.eventValues[slot] unsignedLongLongValue];
    // Gates command-allocator reset for the secondary-viewport path (RenderWindow signals). VB/IB lifetime is the per-viewport ring, not this wait.
    [bd->SharedMetalContext.events[slot] waitUntilSignaledValue:waitValue timeoutMS:UINT64_MAX];
    [bd->SharedMetalContext.commandAllocators[slot] reset];
}

static void ImGui_ImplMetal4_SetupRenderState(ImDrawData* draw_data, id<MTL4CommandBuffer> commandBuffer,
    id<MTL4RenderCommandEncoder> commandEncoder, id<MTLRenderPipelineState> renderPipelineState,
    id<MTLBuffer> vertexBuffer, size_t vertexBufferOffset)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    id<MTL4ArgumentTable> vertexTable = bd->CurrentVertexArgumentTable;
    id<MTL4ArgumentTable> fragmentTable = bd->CurrentFragmentArgumentTable;
    if (vertexTable == nil || fragmentTable == nil)
        [bd->SharedMetalContext vertexArgumentTable:&vertexTable fragmentArgumentTable:&fragmentTable forCommandBuffer:commandBuffer];
    [commandEncoder setCullMode:MTLCullModeNone];
    [commandEncoder setDepthStencilState:bd->SharedMetalContext.depthStencilState];

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to
    // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
    MTLViewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = (double)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x),
        .height = (double)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y),
        .znear = 0.0,
        .zfar = 1.0
    };
    [commandEncoder setViewport:viewport];

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    float N = (float)viewport.znear;
    float F = (float)viewport.zfar;
    const float ortho_projection[4][4] =
    {
        { 2.0f/(R-L),   0.0f,           0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),     0.0f,   0.0f },
        { 0.0f,         0.0f,        1/(F-N),   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T), N/(F-N),   1.0f },
    };

    int currentChunk = (int)((float)bd->SharedMetalContext.currentConstantBufferIndex / (float)METAL_IMGUI_VIEWPORTS_PER_CHUNK);
    int currentIndex = bd->SharedMetalContext.currentConstantBufferIndex % METAL_IMGUI_VIEWPORTS_PER_CHUNK;

    int currentFrameIndex = (int)bd->SharedMetalContext.currentFrameSlot;
    id<MTLBuffer> constantBuffer = bd->SharedMetalContext.constantBuffers[currentFrameIndex][currentChunk];
    ImGui_Metal4_ConstantData* constantBufferContents = (ImGui_Metal4_ConstantData*)constantBuffer.contents;

    memcpy(&constantBufferContents->slots[currentIndex].ModelViewProjectionMatrix, ortho_projection, sizeof(ortho_projection));

    // Offset must stay 256-byte aligned for constant [[buffer(1)]] fetches.
    [vertexTable setAddress:constantBuffer.gpuAddress + (uint64_t)currentIndex * sizeof(ImGui_Metal4_ConstantSlot) atIndex:1];
    // Fixed stride lives in the PSO vertex descriptor (sizeof(ImDrawVert)).
    [vertexTable setAddress:(vertexBuffer.gpuAddress + vertexBufferOffset) atIndex:0];
    [fragmentTable setSamplerState:bd->SharedMetalContext.samplerStateLinear.gpuResourceID atIndex:0];
    [commandEncoder setArgumentTable:vertexTable atStages:MTLRenderStageVertex];
    [commandEncoder setArgumentTable:fragmentTable atStages:MTLRenderStageFragment];
    [commandEncoder setRenderPipelineState:renderPipelineState];
}

static void ImGui_ImplMetal4_DrawCallback_ResetRenderState(const ImDrawList*, const ImDrawCmd*)  {} // Intentionally empty. Used as an identifier for rendering loop to call its code. Simpler to implement this way.
static void ImGui_ImplMetal4_DrawCallback_SetSamplerLinear(const ImDrawList*, const ImDrawCmd*)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    IM_ASSERT(bd->CurrentFragmentArgumentTable != nil);
    [bd->CurrentFragmentArgumentTable setSamplerState:bd->SharedMetalContext.samplerStateLinear.gpuResourceID atIndex:0];
}
static void ImGui_ImplMetal4_DrawCallback_SetSamplerNearest(const ImDrawList*, const ImDrawCmd*)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    IM_ASSERT(bd->CurrentFragmentArgumentTable != nil);
    [bd->CurrentFragmentArgumentTable setSamplerState:bd->SharedMetalContext.samplerStateNearest.gpuResourceID atIndex:0];
}

void ImGui_ImplMetal4_RenderDrawData(ImDrawData* draw_data, id<MTL4CommandBuffer> commandBuffer, id<MTL4RenderCommandEncoder> commandEncoder)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    MetalContext* ctx = bd->SharedMetalContext;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0 || draw_data->TotalVtxCount == 0)
        return;

    // Catch up with texture updates. Most of the times, the list will have 1 element with an OK status, aka nothing to do.
    // (This almost always points to ImGui::GetPlatformIO().Textures[] but is part of ImDrawData to allow overriding or disabling texture updates).
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
            {
                ImGui_ImplMetal4_UpdateTexture(tex);
            }

    // Try to retrieve a render pipeline state that is compatible with the framebuffer config for this frame
    // The hit rate for this cache should be very near 100%.
    id<MTLRenderPipelineState> renderPipelineState = ctx.renderPipelineStateCache[ctx.framebufferDescriptor];
    if (renderPipelineState == nil)
    {
        // No luck; make a new render pipeline state
        renderPipelineState = [ctx renderPipelineStateForFramebufferDescriptor:ctx.framebufferDescriptor device:commandBuffer.device];

        // Cache render pipeline state for later reuse
        ctx.renderPipelineStateCache[ctx.framebufferDescriptor] = renderPipelineState;
    }

    // Per-viewport VB/IB ring keyed to backend currentFrameSlot (advanced in NewFrame).
    IM_ASSERT(draw_data->OwnerViewport != nullptr);
    ImGuiViewportDataMetal* viewport_data = (ImGuiViewportDataMetal*)draw_data->OwnerViewport->RendererUserData;
    IM_ASSERT(viewport_data != nullptr && "Viewport missing RendererUserData — main viewport should be created in Init()");
    ImGuiMetal4WindowRenderBuffers* wrb = ImGui_ImplMetal4_EnsureWindowRenderBuffers(viewport_data);
    ImGuiMetal4FrameRenderBuffers* rb = ImGui_ImplMetal4_FrameRenderBuffersForSlot(wrb, ctx.currentFrameSlot);

    NSUInteger vertexBufferLength = (NSUInteger)draw_data->TotalVtxCount * sizeof(ImDrawVert);
    NSUInteger indexBufferLength = (NSUInteger)draw_data->TotalIdxCount * sizeof(ImDrawIdx);
    if (rb.vertexBuffer == nil || rb.vertexBufferSize < vertexBufferLength)
        ImGui_ImplMetal4_CreateOrResizeBuffer(rb, YES, vertexBufferLength);
    if (rb.indexBuffer == nil || rb.indexBufferSize < indexBufferLength)
        ImGui_ImplMetal4_CreateOrResizeBuffer(rb, NO, indexBufferLength);

    id<MTLBuffer> vertexBuffer = rb.vertexBuffer;
    id<MTLBuffer> indexBuffer = rb.indexBuffer;

    // Wire residency before encoding draws. requestResidency (once at init) makes subsequent
    // commit() calls actually page resources in; useResidencySet covers this CB even if the
    // queue attachment is insufficient for Astral's beginCommandBuffer path.
    [ctx.residencySet commit];
    [commandBuffer useResidencySet:ctx.residencySet];

    bd->RenderCommandEncoder = commandEncoder;
    [ctx vertexArgumentTable:&bd->CurrentVertexArgumentTable
       fragmentArgumentTable:&bd->CurrentFragmentArgumentTable
            forCommandBuffer:commandBuffer];

    // check if more chunks are required
    const int chunksRequired = 1 + (int)((float)bd->SharedMetalContext.currentConstantBufferIndex / (float)METAL_IMGUI_VIEWPORTS_PER_CHUNK);
    if (chunksRequired > bd->SharedMetalContext.constantBufferChunkCount)
    {
        for (NSUInteger i = 0; i < bd->SharedMetalContext.framesInFlight; i++)
        {
            id<MTLBuffer> buffer = [bd->SharedMetalContext.device newBufferWithLength:sizeof(ImGui_Metal4_ConstantData) options:MTLResourceStorageModeShared];
            [bd->SharedMetalContext.constantBuffers[i] addObject:buffer];
            [bd->SharedMetalContext.residencySet addAllocation:buffer];
        }
        bd->SharedMetalContext.constantBufferChunkCount++;
        [ctx.residencySet commit];
    }

    ImGui_ImplMetal4_SetupRenderState(draw_data, commandBuffer, commandEncoder, renderPipelineState, vertexBuffer, 0);
    bd->SharedMetalContext.currentConstantBufferIndex++;

    // Upload all draw lists first (Vulkan pattern), then encode draws — avoids interleaved CPU writes
    // while the encoder is building indexed draws against the same Shared buffers.
    size_t vertexBufferOffset = 0;
    size_t indexBufferOffset = 0;
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        memcpy((char*)vertexBuffer.contents + vertexBufferOffset, draw_list->VtxBuffer.Data, (size_t)draw_list->VtxBuffer.Size * sizeof(ImDrawVert));
        memcpy((char*)indexBuffer.contents + indexBufferOffset, draw_list->IdxBuffer.Data, (size_t)draw_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        vertexBufferOffset += (size_t)draw_list->VtxBuffer.Size * sizeof(ImDrawVert);
        indexBufferOffset += (size_t)draw_list->IdxBuffer.Size * sizeof(ImDrawIdx);
    }

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Metal3 passes TotalIdxCount*sizeof as indexBufferLength on every draw (even with an offset address).
    // Match that: Metal 4 uses this length for index-fetch bounds; too-small length clamps vertex_id to 0
    // (spikes to the first vertex — can look like a lightning bolt toward cursor/UI origin).
    const NSUInteger totalIndexBytes = indexBufferLength;

    size_t global_vtx_offset = 0;
    size_t global_idx_offset = 0;
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                if (pcmd->UserCallback == ImGui_ImplMetal4_DrawCallback_ResetRenderState)
                    ImGui_ImplMetal4_SetupRenderState(draw_data, commandBuffer, commandEncoder, renderPipelineState, vertexBuffer, global_vtx_offset);
                else
                    pcmd->UserCallback(draw_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                // Clamp to viewport as setScissorRect() won't accept values that are off bounds
                if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                if (clip_max.x > (float)fb_width) { clip_max.x = (float)fb_width; }
                if (clip_max.y > (float)fb_height) { clip_max.y = (float)fb_height; }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;
                if (pcmd->ElemCount == 0) // drawIndexedPrimitives() validation doesn't accept this
                    continue;

                // Apply scissor/clipping rectangle
                MTLScissorRect scissorRect =
                {
                    .x = NSUInteger(clip_min.x),
                    .y = NSUInteger(clip_min.y),
                    .width = NSUInteger(clip_max.x - clip_min.x),
                    .height = NSUInteger(clip_max.y - clip_min.y)
                };
                [commandEncoder setScissorRect:scissorRect];

                // Bind texture, Draw
                ImTextureID tex_id = pcmd->GetTexID();
                if (tex_id != ImTextureID_Invalid)
                {
                    id<MTLTexture> texture = (__bridge id<MTLTexture>)(void*)(intptr_t)tex_id;
                    [bd->SharedMetalContext.residencySet addAllocation:texture];
                    [bd->CurrentFragmentArgumentTable setTexture:texture.gpuResourceID atIndex:0];
                }

                // Vulkan-style: bind draw-list VB base, apply VtxOffset as baseVertex (not address rebase).
                [bd->CurrentVertexArgumentTable setAddress:(vertexBuffer.gpuAddress + global_vtx_offset) atIndex:0];

                size_t indexBufferCmdOffset = global_idx_offset + (pcmd->IdxOffset * sizeof(ImDrawIdx));
                [commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                   indexCount:pcmd->ElemCount
                                    indexType:sizeof(ImDrawIdx) == 2 ? MTLIndexTypeUInt16 : MTLIndexTypeUInt32
                                  indexBuffer:indexBuffer.gpuAddress + indexBufferCmdOffset
                            indexBufferLength:totalIndexBytes
                                instanceCount:1
                                   baseVertex:(NSInteger)pcmd->VtxOffset
                                 baseInstance:0];
            }
        }

        global_vtx_offset += (size_t)draw_list->VtxBuffer.Size * sizeof(ImDrawVert);
        global_idx_offset += (size_t)draw_list->IdxBuffer.Size * sizeof(ImDrawIdx);
    }

    // Commit residency set before Astral submits the CB (queue already has this set attached).
    [bd->SharedMetalContext.residencySet commit];
    bd->RenderCommandEncoder = nil;
    bd->CurrentVertexArgumentTable = nil;
    bd->CurrentFragmentArgumentTable = nil;
}

static void ImGui_ImplMetal4_DestroyTexture(ImTextureData* tex)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    if (MetalTexture* backend_tex = (__bridge_transfer MetalTexture*)(tex->BackendUserData))
    {
        IM_ASSERT(backend_tex.metalTexture == (__bridge id<MTLTexture>)(void*)(intptr_t)tex->TexID);
        if (backend_tex.metalTexture != nil && bd != nullptr && bd->SharedMetalContext.residencySet != nil)
        {
            [bd->SharedMetalContext.residencySet removeAllocation:backend_tex.metalTexture];
            [bd->SharedMetalContext.residencySet commit];
        }
        backend_tex.metalTexture = nil;

        // Clear identifiers and mark as destroyed (in order to allow e.g. calling InvalidateDeviceObjects while running)
        tex->SetTexID(ImTextureID_Invalid);
        tex->BackendUserData = nullptr;
    }
    tex->SetStatus(ImTextureStatus_Destroyed);
}

static id<MTLTexture> ImGui_ImplMetal4_CreateSharedAtlasTexture(ImGui_ImplMetal4_Data* bd, ImTextureData* tex)
{
    IM_ASSERT(tex->Format == ImTextureFormat_RGBA32);
    IM_ASSERT(tex->Pixels != nullptr);

    MTLTextureDescriptor* textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                 width:(NSUInteger)tex->Width
                                                                                                height:(NSUInteger)tex->Height
                                                                                             mipmapped:NO];
    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.storageMode = MTLStorageModeShared;

    id<MTLTexture> texture = [bd->SharedMetalContext.device newTextureWithDescriptor:textureDescriptor];
    [bd->SharedMetalContext.residencySet addAllocation:texture];
    // Full CPU upload — ImGui already applied WantUpdates into tex->Pixels before Status is set.
    [texture replaceRegion:MTLRegionMake2D(0, 0, (NSUInteger)tex->Width, (NSUInteger)tex->Height)
               mipmapLevel:0
                 withBytes:tex->Pixels
               bytesPerRow:(NSUInteger)tex->Width * 4];
    return texture;
}

static void ImGui_ImplMetal4_RetireTexture(MetalContext* ctx, id<MTLTexture> texture)
{
    if (texture == nil || ctx == nil)
        return;
    IM_ASSERT(ctx.retiredTextures != nil && ctx.retiredTextures.count == ctx.framesInFlight);
    [ctx.retiredTextures[ctx.currentFrameSlot] addObject:texture];
}

static void ImGui_ImplMetal4_FlushRetiredTextures(MetalContext* ctx, NSUInteger slot)
{
    if (ctx == nil || ctx.retiredTextures == nil || slot >= ctx.retiredTextures.count)
        return;
    NSMutableArray<id<MTLTexture>>* bucket = ctx.retiredTextures[slot];
    if (bucket.count == 0)
        return;
    id<MTLResidencySet> residencySet = ctx.residencySet;
    for (id<MTLTexture> texture in bucket)
    {
        if (residencySet != nil)
            [residencySet removeAllocation:texture];
    }
    [bucket removeAllObjects];
    if (residencySet != nil)
        [residencySet commit];
}

static void ImGui_ImplMetal4_FlushAllRetiredTextures(MetalContext* ctx)
{
    if (ctx == nil || ctx.retiredTextures == nil)
        return;
    for (NSUInteger i = 0; i < ctx.retiredTextures.count; i++)
        ImGui_ImplMetal4_FlushRetiredTextures(ctx, i);
}

void ImGui_ImplMetal4_UpdateTexture(ImTextureData* tex)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        // Create and upload new texture to graphics system
        //IMGUI_DEBUG_LOG("UpdateTexture #%03d: WantCreate %dx%d\n", tex->UniqueID, tex->Width, tex->Height);
        IM_ASSERT(tex->TexID == ImTextureID_Invalid && tex->BackendUserData == nullptr);
        IM_ASSERT(tex->Format == ImTextureFormat_RGBA32);

        // We are retrieving and uploading the font atlas as a 4-channels RGBA texture here.
        // In theory we could call GetTexDataAsAlpha8() and upload a 1-channel texture to save on memory access bandwidth.
        // However, using a shader designed for 1-channel texture would make it less obvious to use the ImTextureID facility to render users own textures.
        // You can make that change in your implementation.
        id<MTLTexture> texture = ImGui_ImplMetal4_CreateSharedAtlasTexture(bd, tex);
        MetalTexture* backend_tex = [[MetalTexture alloc] initWithTexture:texture];

        // Store identifiers
        tex->SetTexID((ImTextureID)(intptr_t)(__bridge void*)texture);
        tex->SetStatus(ImTextureStatus_OK);
        tex->BackendUserData = (__bridge_retained void*)(backend_tex);
    }
    else if (tex->Status == ImTextureStatus_WantUpdates)
    {
        // Copy-on-write: never replaceRegion the live Shared texture while prior FIF frames may still sample it.
        // Upload full atlas from CPU pixels (already includes Updates[]) onto a new texture, then swap TexID.
        MetalTexture* backend_tex = (__bridge MetalTexture*)(tex->BackendUserData);
        IM_ASSERT(backend_tex != nil && backend_tex.metalTexture != nil);
        id<MTLTexture> old_texture = backend_tex.metalTexture;
        id<MTLTexture> new_texture = ImGui_ImplMetal4_CreateSharedAtlasTexture(bd, tex);
        ImGui_ImplMetal4_RetireTexture(bd->SharedMetalContext, old_texture);
        backend_tex.metalTexture = new_texture;
        tex->SetTexID((ImTextureID)(intptr_t)(__bridge void*)new_texture);
        tex->SetStatus(ImTextureStatus_OK);
    }
    else if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames >= (int)bd->SharedMetalContext.framesInFlight)
    {
        ImGui_ImplMetal4_DestroyTexture(tex);
    }
}

bool ImGui_ImplMetal4_CreateDeviceObjects(id<MTLDevice> device)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();

    MTLResidencySetDescriptor* residencySetDescriptor = [[MTLResidencySetDescriptor alloc] init];
    residencySetDescriptor.initialCapacity = 1000;

    NSError* error = nil;
    bd->SharedMetalContext.residencySet = [device newResidencySetWithDescriptor:residencySetDescriptor error:&error];
    IM_ASSERT(bd->SharedMetalContext.residencySet != nil && error == nil);

    [bd->SharedMetalContext.commandQueue addResidencySet:bd->SharedMetalContext.residencySet];

    MTLDepthStencilDescriptor* depthStencilDescriptor = [[MTLDepthStencilDescriptor alloc] init];
    depthStencilDescriptor.depthWriteEnabled = NO;
    depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionAlways;
    bd->SharedMetalContext.depthStencilState = [device newDepthStencilStateWithDescriptor:depthStencilDescriptor];
    MTLSamplerDescriptor* samplerDescriptor = [[MTLSamplerDescriptor alloc] init];
    samplerDescriptor.supportArgumentBuffers = YES;
    // Match Vulkan ImGui: clamp edges so atlas UV fringes don't wrap (pixel fonts).
    samplerDescriptor.sAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDescriptor.tAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDescriptor.rAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDescriptor.minFilter = MTLSamplerMinMagFilterLinear;
    samplerDescriptor.magFilter = MTLSamplerMinMagFilterLinear;
    samplerDescriptor.mipFilter = MTLSamplerMipFilterLinear;
    bd->SharedMetalContext.samplerStateLinear = [device newSamplerStateWithDescriptor:samplerDescriptor];
    samplerDescriptor.minFilter = MTLSamplerMinMagFilterNearest;
    samplerDescriptor.magFilter = MTLSamplerMinMagFilterNearest;
    samplerDescriptor.mipFilter = MTLSamplerMipFilterNearest;
    bd->SharedMetalContext.samplerStateNearest = [device newSamplerStateWithDescriptor:samplerDescriptor];

    NSMutableArray<id<MTL4CommandAllocator>>* commandAllocators = [NSMutableArray array];
    NSMutableArray<id<MTLSharedEvent>>* events = [NSMutableArray array];
    NSMutableArray<NSNumber*>* eventValues = [NSMutableArray array];
    bd->SharedMetalContext.constantBuffers = [NSMutableArray array];
    bd->SharedMetalContext.retiredTextures = [NSMutableArray array];
    for (NSUInteger i = 0; i < bd->SharedMetalContext.framesInFlight; i++)
    {
        events[i] = [device newSharedEvent];
        commandAllocators[i] = [device newCommandAllocator];
        eventValues[i] = @0;
        bd->SharedMetalContext.constantBuffers[i] = [NSMutableArray array];
        [bd->SharedMetalContext.retiredTextures addObject:[NSMutableArray array]];
        id<MTLBuffer> buffer = [device newBufferWithLength:sizeof(ImGui_Metal4_ConstantData) options:MTLResourceStorageModeShared];
        [bd->SharedMetalContext.constantBuffers[i] addObject:buffer];
        [bd->SharedMetalContext.residencySet addAllocation:buffer];
    }
    bd->SharedMetalContext.constantBufferChunkCount = 1;
    bd->SharedMetalContext.events = events;
    bd->SharedMetalContext.eventValues = eventValues;
    bd->SharedMetalContext.commandAllocators = commandAllocators;
    // Argument tables are created lazily per command buffer in -vertexArgumentTable:fragmentArgumentTable:forCommandBuffer:

    // Persistently wire the set so future commit() calls make new allocations resident (Metal3 does this).
    [bd->SharedMetalContext.residencySet commit];
    [bd->SharedMetalContext.residencySet requestResidency];

    ImGui_ImplMetal_CreateDeviceObjectsForPlatformWindows();
    return true;
}

void ImGui_ImplMetal4_DestroyDeviceObjects()
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();

    // Drop CoW-retired atlases before destroying live textures / residency set.
    ImGui_ImplMetal4_FlushAllRetiredTextures(bd->SharedMetalContext);

    // Destroy all textures
    for (ImTextureData* tex : ImGui::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
            ImGui_ImplMetal4_DestroyTexture(tex);

    // Drop VB/IB rings before tearing down the residency set (Metal 4 CBs do not retain resources).
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    for (int n = 0; n < platform_io.Viewports.Size; n++)
    {
        if (ImGuiViewportDataMetal* vd = (ImGuiViewportDataMetal*)platform_io.Viewports[n]->RendererUserData)
        {
            ImGui_ImplMetal4_DestroyWindowRenderBuffers(vd->RenderBuffers);
            vd->RenderBuffers = nil;
        }
    }

    [bd->SharedMetalContext.renderPipelineStateCache removeAllObjects];
    [bd->SharedMetalContext.vertexArgumentTablesForCommandBuffers removeAllObjects];
    [bd->SharedMetalContext.fragmentArgumentTablesForCommandBuffers removeAllObjects];
    bd->CurrentVertexArgumentTable = nil;
    bd->CurrentFragmentArgumentTable = nil;
    bd->SharedMetalContext.samplerStateLinear = nil;
    bd->SharedMetalContext.samplerStateNearest = nil;
    ImGui_ImplMetal_InvalidateDeviceObjectsForPlatformWindows();
    [bd->SharedMetalContext.renderPipelineStateCache removeAllObjects];
}

bool ImGui_ImplMetal4_Init(id<MTLDevice> device, id<MTL4CommandQueue> commandQueue, int framesInFlight)
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");
    IM_ASSERT(framesInFlight > 0 && "framesInFlight must be at least 1!");

    ImGui_ImplMetal4_Data* bd = IM_NEW(ImGui_ImplMetal4_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_metal4";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;   // We can honor ImGuiPlatformIO::Textures[] requests during render.
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;  // We can create multi-viewports on the Renderer side (optional)

    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.DrawCallback_ResetRenderState = ImGui_ImplMetal4_DrawCallback_ResetRenderState;
    platform_io.DrawCallback_SetSamplerLinear = ImGui_ImplMetal4_DrawCallback_SetSamplerLinear;
    platform_io.DrawCallback_SetSamplerNearest = ImGui_ImplMetal4_DrawCallback_SetSamplerNearest;

    bd->SharedMetalContext = [[MetalContext alloc] init];
    bd->SharedMetalContext.device = device;
    bd->SharedMetalContext.commandQueue = commandQueue;
    bd->SharedMetalContext.framesInFlight = (NSUInteger)framesInFlight;
    // First NewFrame advances to 0.
    bd->SharedMetalContext.currentFrameSlot = (NSUInteger)framesInFlight - 1;

    // RenderDrawData expects RendererUserData to hold the per-viewport VB/IB ring (main has no platform window data).
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->RendererUserData = IM_NEW(ImGuiViewportDataMetal)();

    ImGui_ImplMetal_InitMultiViewportSupport();
    return true;
}

void ImGui_ImplMetal4_Shutdown()
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    IM_UNUSED(bd);
    IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

    ImGui_ImplMetal_ShutdownMultiViewportSupport();
    ImGui_ImplMetal4_DestroyDeviceObjects();

    // Manually delete main viewport render data (DestroyPlatformWindows does not own it).
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    if (ImGuiViewportDataMetal* vd = (ImGuiViewportDataMetal*)main_viewport->RendererUserData)
    {
        ImGui_ImplMetal4_DestroyWindowRenderBuffers(vd->RenderBuffers);
        vd->RenderBuffers = nil;
        IM_DELETE(vd);
    }
    main_viewport->RendererUserData = nullptr;

    ImGui_ImplMetal4_DestroyBackendData();

    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_RendererHasVtxOffset | ImGuiBackendFlags_RendererHasTextures | ImGuiBackendFlags_RendererHasViewports);
    platform_io.ClearRendererHandlers();
}

@implementation ImGuiMetal4FrameRenderBuffers
@end

@implementation ImGuiMetal4WindowRenderBuffers
@end

static void ImGui_ImplMetal4_CreateOrResizeBuffer(ImGuiMetal4FrameRenderBuffers* rb, BOOL is_vertex, NSUInteger new_size)
{
    IM_ASSERT(rb != nil);
    IM_ASSERT(new_size > 0);
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    id<MTLResidencySet> residencySet = bd->SharedMetalContext.residencySet;
    id<MTLBuffer> old_buffer = is_vertex ? rb.vertexBuffer : rb.indexBuffer;
    // Safe under FIF ring: this slot is only rewritten after framesInFlight frames, so the GPU is done with the old buffer.
    if (old_buffer != nil)
        [residencySet removeAllocation:old_buffer];

    id<MTLBuffer> new_buffer = [bd->SharedMetalContext.device newBufferWithLength:new_size options:MTLResourceStorageModeShared];
    IM_ASSERT(new_buffer != nil);
    [residencySet addAllocation:new_buffer];
    if (is_vertex)
    {
        rb.vertexBuffer = new_buffer;
        rb.vertexBufferSize = new_size;
    }
    else
    {
        rb.indexBuffer = new_buffer;
        rb.indexBufferSize = new_size;
    }
}

static void ImGui_ImplMetal4_DestroyWindowRenderBuffers(ImGuiMetal4WindowRenderBuffers* wrb)
{
    if (wrb == nil)
        return;
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    id<MTLResidencySet> residencySet = bd ? bd->SharedMetalContext.residencySet : nil;
    for (ImGuiMetal4FrameRenderBuffers* rb in wrb.frames)
    {
        if (rb.vertexBuffer != nil)
        {
            [residencySet removeAllocation:rb.vertexBuffer];
            rb.vertexBuffer = nil;
        }
        if (rb.indexBuffer != nil)
        {
            [residencySet removeAllocation:rb.indexBuffer];
            rb.indexBuffer = nil;
        }
        rb.vertexBufferSize = 0;
        rb.indexBufferSize = 0;
    }
    [wrb.frames removeAllObjects];
    wrb.index = 0;
}

static ImGuiMetal4WindowRenderBuffers* ImGui_ImplMetal4_EnsureWindowRenderBuffers(ImGuiViewportDataMetal* viewport_data)
{
    IM_ASSERT(viewport_data != nil);
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    NSUInteger count = bd->SharedMetalContext.framesInFlight;
    IM_ASSERT(count > 0);
    if (viewport_data->RenderBuffers == nil)
    {
        ImGuiMetal4WindowRenderBuffers* wrb = [[ImGuiMetal4WindowRenderBuffers alloc] init];
        wrb.index = 0;
        wrb.frames = [NSMutableArray arrayWithCapacity:count];
        for (NSUInteger i = 0; i < count; i++)
            [wrb.frames addObject:[[ImGuiMetal4FrameRenderBuffers alloc] init]];
        viewport_data->RenderBuffers = wrb;
    }
    IM_ASSERT(viewport_data->RenderBuffers.frames.count == count);
    return viewport_data->RenderBuffers;
}

static ImGuiMetal4FrameRenderBuffers* ImGui_ImplMetal4_FrameRenderBuffersForSlot(ImGuiMetal4WindowRenderBuffers* wrb, NSUInteger slot)
{
    IM_ASSERT(wrb != nil && wrb.frames.count > 0);
    IM_ASSERT(slot < wrb.frames.count);
    return wrb.frames[slot];
}

#pragma mark - FramebufferDescriptor implementation

@implementation FramebufferDescriptor
- (instancetype)initWithRenderPassDescriptor:(AttachmentFormats)attachmentFormats
{
    if ((self = [super init]))
    {
        _sampleCount = attachmentFormats.SampleCount;
        _colorPixelFormat = (MTLPixelFormat)attachmentFormats.ColorPixelFormat;
        _depthPixelFormat = (MTLPixelFormat)attachmentFormats.DepthPixelFormat;
        _stencilPixelFormat = (MTLPixelFormat)attachmentFormats.StencilPixelFormat;
    }
    return self;
}

- (nonnull id)copyWithZone:(nullable NSZone*)zone
{
    FramebufferDescriptor* copy = [[FramebufferDescriptor allocWithZone:zone] init];
    copy.sampleCount = self.sampleCount;
    copy.colorPixelFormat = self.colorPixelFormat;
    copy.depthPixelFormat = self.depthPixelFormat;
    copy.stencilPixelFormat = self.stencilPixelFormat;
    return copy;
}

- (NSUInteger)hash
{
    NSUInteger sc = _sampleCount & 0x3;
    NSUInteger cf = _colorPixelFormat & 0x3FF;
    NSUInteger df = _depthPixelFormat & 0x3FF;
    NSUInteger sf = _stencilPixelFormat & 0x3FF;
    NSUInteger hash = (sf << 22) | (df << 12) | (cf << 2) | sc;
    return hash;
}

- (BOOL)isEqual:(id)object
{
    FramebufferDescriptor* other = object;
    if (![other isKindOfClass:[FramebufferDescriptor class]])
        return NO;
    return other.sampleCount == self.sampleCount      &&
    other.colorPixelFormat   == self.colorPixelFormat &&
    other.depthPixelFormat   == self.depthPixelFormat &&
    other.stencilPixelFormat == self.stencilPixelFormat;
}

@end

#pragma mark - MetalTexture implementation

@implementation MetalTexture
- (instancetype)initWithTexture:(id<MTLTexture>)metalTexture
{
    if ((self = [super init]))
        self.metalTexture = metalTexture;
    return self;
}

@end

#pragma mark - MetalContext implementation

@implementation MetalContext
- (instancetype)init
{
    if ((self = [super init]))
    {
        self.renderPipelineStateCache = [NSMutableDictionary dictionary];
        // Weak keys: do not extend CB lifetime; entry drops when the CB is released.
        self.vertexArgumentTablesForCommandBuffers = [NSMapTable weakToStrongObjectsMapTable];
        self.fragmentArgumentTablesForCommandBuffers = [NSMapTable weakToStrongObjectsMapTable];
    }
    return self;
}

- (void)dealloc
{
}

- (void)vertexArgumentTable:(id<MTL4ArgumentTable>*)outVertex
    fragmentArgumentTable:(id<MTL4ArgumentTable>*)outFragment
       forCommandBuffer:(id<MTL4CommandBuffer>)commandBuffer
{
    IM_ASSERT(commandBuffer != nil);
    IM_ASSERT(outVertex != nil && outFragment != nil);

    id<MTL4ArgumentTable> vertexTable = [self.vertexArgumentTablesForCommandBuffers objectForKey:commandBuffer];
    id<MTL4ArgumentTable> fragmentTable = [self.fragmentArgumentTablesForCommandBuffers objectForKey:commandBuffer];
    if (vertexTable != nil && fragmentTable != nil)
    {
        *outVertex = vertexTable;
        *outFragment = fragmentTable;
        return;
    }

    NSError* error = nil;

    MTL4ArgumentTableDescriptor* vertexDesc = [[MTL4ArgumentTableDescriptor alloc] init];
    vertexDesc.maxBufferBindCount = 2; // VB @0 + constants @1
    vertexDesc.maxTextureBindCount = 1; // unused; keep ≥1 for descriptor validity
    vertexDesc.maxSamplerStateBindCount = 1;
    vertexDesc.supportAttributeStrides = NO;
    vertexDesc.initializeBindings = YES;
    vertexTable = [self.device newArgumentTableWithDescriptor:vertexDesc error:&error];
    IM_ASSERT(vertexTable != nil && error == nil);

    MTL4ArgumentTableDescriptor* fragmentDesc = [[MTL4ArgumentTableDescriptor alloc] init];
    fragmentDesc.maxBufferBindCount = 1; // unused
    fragmentDesc.maxTextureBindCount = 1;
    fragmentDesc.maxSamplerStateBindCount = 2;
    fragmentDesc.supportAttributeStrides = NO;
    fragmentDesc.initializeBindings = YES;
    fragmentTable = [self.device newArgumentTableWithDescriptor:fragmentDesc error:&error];
    IM_ASSERT(fragmentTable != nil && error == nil);

    [self.vertexArgumentTablesForCommandBuffers setObject:vertexTable forKey:commandBuffer];
    [self.fragmentArgumentTablesForCommandBuffers setObject:fragmentTable forKey:commandBuffer];
    *outVertex = vertexTable;
    *outFragment = fragmentTable;
}

const char* shaderCode = R"(
#include <metal_stdlib>
using namespace metal;

struct Uniforms {
    float4x4 projectionMatrix;
};

struct VertexIn {
    float2 position  [[attribute(0)]];
    float2 texCoords [[attribute(1)]];
    uchar4 color     [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoords;
    float4 color;
};

vertex VertexOut vertex_main(VertexIn in                 [[stage_in]],
                             constant Uniforms &uniforms [[buffer(1)]])
{
    VertexOut out;
    out.position = uniforms.projectionMatrix * float4(in.position, 0, 1);
    out.texCoords = in.texCoords;
    out.color = float4(in.color) / float4(255.0);
    return out;
}

fragment half4 fragment_main(VertexOut in [[stage_in]],
                             texture2d<half, access::sample> texture [[texture(0)]],
                             sampler textureSampler [[sampler(0)]])
{
    half4 texColor = texture.sample(textureSampler, in.texCoords);
    return half4(in.color) * texColor;
}
)";

- (id<MTLRenderPipelineState>)renderPipelineStateForFramebufferDescriptor:(FramebufferDescriptor*)descriptor device:(id<MTLDevice>)device
{
    NSError* error = nil;

    id<MTLLibrary> library = [device newLibraryWithSource:[NSString stringWithUTF8String:shaderCode] options:nil error:&error];
    if (library == nil)
    {
        NSLog(@"Error: failed to create Metal library: %@", error);
        return nil;
    }

    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertex_main"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragment_main"];

    if (vertexFunction == nil || fragmentFunction == nil)
    {
        NSLog(@"Error: failed to find Metal shader functions in library: %@", error);
        return nil;
    }

    MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];
    vertexDescriptor.attributes[0].offset = offsetof(ImDrawVert, pos);
    vertexDescriptor.attributes[0].format = MTLVertexFormatFloat2; // position
    vertexDescriptor.attributes[0].bufferIndex = 0;
    vertexDescriptor.attributes[1].offset = offsetof(ImDrawVert, uv);
    vertexDescriptor.attributes[1].format = MTLVertexFormatFloat2; // texCoords
    vertexDescriptor.attributes[1].bufferIndex = 0;
    vertexDescriptor.attributes[2].offset = offsetof(ImDrawVert, col);
    vertexDescriptor.attributes[2].format = MTLVertexFormatUChar4; // color
    vertexDescriptor.attributes[2].bufferIndex = 0;
    vertexDescriptor.layouts[0].stepRate = 1;
    vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    vertexDescriptor.layouts[0].stride = sizeof(ImDrawVert);

    MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.vertexDescriptor = vertexDescriptor;
    pipelineDescriptor.rasterSampleCount = self.framebufferDescriptor.sampleCount;
    pipelineDescriptor.colorAttachments[0].pixelFormat = self.framebufferDescriptor.colorPixelFormat;
    pipelineDescriptor.colorAttachments[0].blendingEnabled = YES;
    pipelineDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    pipelineDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    pipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipelineDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    pipelineDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
    pipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipelineDescriptor.depthAttachmentPixelFormat = self.framebufferDescriptor.depthPixelFormat;
    pipelineDescriptor.stencilAttachmentPixelFormat = self.framebufferDescriptor.stencilPixelFormat;

    id<MTLRenderPipelineState> renderPipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
    if (error != nil)
        NSLog(@"Error: failed to create Metal pipeline state: %@", error);

    return renderPipelineState;
}

@end


#pragma mark - Multi-viewport support

#import <QuartzCore/CAMetalLayer.h>

#if TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#endif

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the back-end to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

static void ImGui_ImplMetal_CreateWindow(ImGuiViewport* viewport)
{
    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    ImGuiViewportDataMetal* data = IM_NEW(ImGuiViewportDataMetal)();
    viewport->RendererUserData = data;

    // PlatformHandleRaw should always be a NSWindow*, whereas PlatformHandle might be a higher-level handle (e.g. GLFWWindow*, SDL_Window*).
    // Some back-ends will leave PlatformHandleRaw == 0, in which case we assume PlatformHandle will contain the NSWindow*.
    void* handle = viewport->PlatformHandleRaw ? viewport->PlatformHandleRaw : viewport->PlatformHandle;
    IM_ASSERT(handle != nullptr);

    id<MTLDevice> device = bd->SharedMetalContext.device;
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = device;
    layer.framebufferOnly = YES;
    layer.pixelFormat = bd->SharedMetalContext.framebufferDescriptor.colorPixelFormat;
#if TARGET_OS_OSX
    NSWindow* window = (__bridge NSWindow*)handle;
    NSView* view = window.contentView;
    view.layer = layer;
    view.wantsLayer = YES;
#endif
    data->MetalLayer = layer;
    data->CommandQueue = [device newCommandQueue];
    data->RenderPassDescriptor = [[MTL4RenderPassDescriptor alloc] init];
    data->Handle = handle;
}

static void ImGui_ImplMetal_DestroyWindow(ImGuiViewport* viewport)
{
    if (ImGuiViewportDataMetal* data = (ImGuiViewportDataMetal*)viewport->RendererUserData)
    {
        // Dropping the CB key removes its argument-table map entry (weak-to-strong NSMapTable).
        data->CommandBuffer = nil;
        data->CommandQueue = nil;
        data->MetalLayer = nil;
        data->RenderPassDescriptor = nil;
        ImGui_ImplMetal4_DestroyWindowRenderBuffers(data->RenderBuffers);
        data->RenderBuffers = nil;
        IM_DELETE(data);
    }
    viewport->RendererUserData = nullptr;
}

inline static CGSize MakeScaledSize(CGSize size, CGFloat scale)
{
    return CGSizeMake(size.width * scale, size.height * scale);
}

static void ImGui_ImplMetal_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
    ImGuiViewportDataMetal* data = (ImGuiViewportDataMetal*)viewport->RendererUserData;
    CGFloat scale = (CGFloat)viewport->DpiScale;
#if TARGET_OS_OSX
    void* handle = viewport->PlatformHandleRaw ? viewport->PlatformHandleRaw : viewport->PlatformHandle;
    NSWindow* window = (__bridge NSWindow*)handle;
    scale = window.backingScaleFactor;
    data->MetalLayer.contentsScale = scale;
#endif
    data->MetalLayer.drawableSize = MakeScaledSize(CGSizeMake(size.x, size.y), scale);
}

static void ImGui_ImplMetal_RenderWindow(ImGuiViewport* viewport, void*)
{
    ImGuiViewportDataMetal* data = (ImGuiViewportDataMetal*)viewport->RendererUserData;

#if TARGET_OS_OSX
    void* handle = viewport->PlatformHandleRaw ? viewport->PlatformHandleRaw : viewport->PlatformHandle;
    NSWindow* window = (__bridge NSWindow*)handle;

    // Always render the first frame, regardless of occlusionState, to avoid an initial flicker
    if ((window.occlusionState & NSWindowOcclusionStateVisible) == 0 && !data->FirstFrame)
    {
        // Do not render windows which are completely occluded. Calling -[CAMetalLayer nextDrawable] will hang for
        // approximately 1 second if the Metal layer is completely occluded.
        return;
    }
    data->FirstFrame = false;

    float fb_scale = (float)window.backingScaleFactor;
    data->MetalLayer.contentsScale = fb_scale;
    data->MetalLayer.drawableSize = MakeScaledSize(CGSizeMake(viewport->Size.x, viewport->Size.y), fb_scale);
#endif

    id <CAMetalDrawable> drawable = [data->MetalLayer nextDrawable];
    if (drawable == nil)
        return;

    MTL4RenderPassDescriptor* renderPassDescriptor = data->RenderPassDescriptor;
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
    if ((viewport->Flags & ImGuiViewportFlags_NoRendererClear) == 0)
        renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;

    ImGui_ImplMetal4_Data* bd = ImGui_ImplMetal4_GetBackendData();
    if (data->CommandBuffer == nil)
        data->CommandBuffer = [bd->SharedMetalContext.device newCommandBuffer];
    id <MTL4CommandBuffer> commandBuffer = data->CommandBuffer;
    [commandBuffer beginCommandBufferWithAllocator:bd->SharedMetalContext.commandAllocators[bd->SharedMetalContext.currentFrameSlot]];

    id <MTL4RenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    ImGui_ImplMetal4_RenderDrawData(viewport->DrawData, commandBuffer, renderEncoder);
    [renderEncoder endEncoding];
    [commandBuffer endCommandBuffer];

    [bd->SharedMetalContext.commandQueue waitForDrawable:drawable];
    [bd->SharedMetalContext.commandQueue commit:&commandBuffer count:1];
    [bd->SharedMetalContext.commandQueue signalDrawable:drawable];
    [drawable present];
    {
        NSUInteger s = bd->SharedMetalContext.currentFrameSlot;
        uint64_t nv = [bd->SharedMetalContext.eventValues[s] unsignedLongLongValue] + 1;
        bd->SharedMetalContext.eventValues[s] = @(nv);
        [bd->SharedMetalContext.commandQueue signalEvent:bd->SharedMetalContext.events[s] value:nv];
    }
}

static void ImGui_ImplMetal_InitMultiViewportSupport()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_CreateWindow = ImGui_ImplMetal_CreateWindow;
    platform_io.Renderer_DestroyWindow = ImGui_ImplMetal_DestroyWindow;
    platform_io.Renderer_SetWindowSize = ImGui_ImplMetal_SetWindowSize;
    platform_io.Renderer_RenderWindow = ImGui_ImplMetal_RenderWindow;
}

static void ImGui_ImplMetal_ShutdownMultiViewportSupport()
{
    ImGui::DestroyPlatformWindows();
}

static void ImGui_ImplMetal_CreateDeviceObjectsForPlatformWindows()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    for (int i = 1; i < platform_io.Viewports.Size; i++)
        if (!platform_io.Viewports[i]->RendererUserData)
            ImGui_ImplMetal_CreateWindow(platform_io.Viewports[i]);
}

static void ImGui_ImplMetal_InvalidateDeviceObjectsForPlatformWindows()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    for (int i = 1; i < platform_io.Viewports.Size; i++)
        if (platform_io.Viewports[i]->RendererUserData)
            ImGui_ImplMetal_DestroyWindow(platform_io.Viewports[i]);
}

//-----------------------------------------------------------------------------


#endif // #ifndef IMGUI_DISABLE
