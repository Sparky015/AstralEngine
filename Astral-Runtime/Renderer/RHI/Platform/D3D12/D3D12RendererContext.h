/**
* @file D3D12RendererContext.h
* @author Andrew Fagan
* @date 3/24/2026
*/

#pragma once

#include "Renderer/RHI/RendererContext.h"

#include "D3D12Helpers.h"
#include <GLFW/glfw3.h>

namespace Astral {

    /**
     * @brief Holds components needed for the lifetime of the D3D12 renderer such as the device
     */
    class D3D12RenderingContext : public RenderingContext
    {
    public:
        explicit D3D12RenderingContext(GLFWwindow* window);
        ~D3D12RenderingContext() override = default;

        /**
         * @brief Initializes the D3D12 rendering context
         */
        void Init() override;

        /**
         * @brief Shuts down the D3D12 rendering context
         */
        void Shutdown() override;

        Device& GetDevice() override;

        uint32 GetNumValidationErrorsAndWarnings() override; // TODO


        void ClearNumValidationErrorsAndWarnings() override; // TODO


        void* GetInstanceHandle() override { return nullptr; } // TODO

        void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) override; // TODO
        void ShutdownImGuiForAPIBackend() override; // TODO

    private:

        void EnableDebugLayer();

        /**
         * @brief Creates the DXGIFactory
         */
        void CreateDXGIFactory();

        /**
         * @brief Destroys the DXGIFactory
         */
        void DestroyDXGIFactory();

        /**
         * @brief
         */
        void CreateDevice(); // TODO

        /**
         * @brief
         */
        void DestroyDevice(); // TODO



        GLFWwindow* m_Window;
        ComPtr<IDXGIFactory4> m_Factory;
        GraphicsOwnedPtr<Device> m_Device;
    };

}

