//
// Created by sparky on 4/16/2024.
//
#pragma once

#include "Solas/Client/UserSetup.h"
#include "Solas/Core/CoreDefinitions.h"
#include "Solas/Core/Layers/LayerStack.h"
#include "Solas/Debug/DebugLayer.h"
#include "Solas/ImGui/ImGuiLayer.h"
#include "Solas/Renderer/RendererLayer.h"
#include "Solas/Window/Window.h"


namespace Ayla::Core
{
    using Ayla::Windows::Window;
    using Ayla::EventManagement::IEvent;
    using Layers::ILayer;
    using Layers::LayerStack;

    /** The brain and backbone of the engine. */
    class Application
    {
    public:
        Application();
        ~Application();

        /** Starts the application loop. */
        void Run() const;

        /** Passes events to the LayerStack. */
        void OnEvent(IEvent&);

        /** Gives access to the Window instance. */
        Window& GetWindow();

        /** Gives access to the LayerStack instance. */
        LayerStack& GetLayerStack();

        /** Gives access to the Application instance. */
        static Application& Get();

        /** Lets the client link their loop to the Application. */
        void SetClientLoop(Client::ClientLoop* clientLoop);

    private:

        bool m_IsAppRunning = true;
        static Application* m_Application;
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<LayerStack> m_LayerStack;
        std::unique_ptr<GUI::ImGuiLayer> m_ImGuiLayer;
        std::unique_ptr<Renderer::RendererLayer> m_RendererLayer;
        std::unique_ptr<Debug::DebugLayer> m_DebugLayer;
        Client::ClientLoop* m_ClientLoop;
    };

} // namespace Ayla::Core


