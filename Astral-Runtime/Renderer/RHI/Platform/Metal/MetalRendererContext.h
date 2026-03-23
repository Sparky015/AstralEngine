/**
* @file MetalRendererContext.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/RendererContext.h"

#include <GLFW/glfw3.h>

namespace Astral {

    class MetalRenderingContext : public RenderingContext
    {
    public:
        explicit MetalRenderingContext(GLFWwindow* window);
        ~MetalRenderingContext() override = default;

        void Init() override;
        void Shutdown() override;

        Device& GetDevice() override;
        uint32 GetNumValidationErrorsAndWarnings() override;
        void ClearNumValidationErrorsAndWarnings() override;

        std::string_view GetRenderingAPI() override;
        std::string_view GetGPUVendor() override;
        std::string_view GetGraphicsProcessorName() override;

        void* GetInstanceHandle() override { return nullptr; }

        void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) override; // TODO
        void ShutdownImGuiForAPIBackend() override; // TODO

    private:

        void CreateDevice();
        void DestroyDevice();

        GraphicsOwnedPtr<Device> m_Device;
        GLFWwindow* m_Window;
    };

}

