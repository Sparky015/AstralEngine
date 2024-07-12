//
// Created by sparky on 4/16/2024.
//
#pragma once

#include "Ayla/Client/UserSetup.h"
#include "Ayla/Core/Core.h"
#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/Debug/DebugLayer.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Window/Window.h"



namespace Ayla::Core
{
    using namespace Ayla::Windows;
    //using Ayla::Events::IEvent;

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
        static Window& GetWindow();

        /** Gives access to the LayerStack instance. */
        static LayerStack& GetLayerStack();

        /** Gives access to the Application instance. */
        static Application& Get();

        /** Lets the client link their loop to the Application. */
        void SetClientLoop(Client::ClientLoop* clientLoop);

    private:

        bool m_IsAppRunning = true;
        static Application* m_Application;
        static std::unique_ptr<Window> m_Window;
        static std::unique_ptr<LayerStack> m_LayerStack;
        std::unique_ptr<GUI::ImGuiLayer> m_ImGuiLayer;
        std::unique_ptr<Debug::DebugLayer> m_DebugLayer;
        Client::ClientLoop* m_ClientLoop;
    };

} // namespace Ayla::Core


