//
// Created by sparky on 4/16/2024.
//
#pragma once

#include "Ayla/Core/Core.h"
#include "Ayla/Layers/LayerStack.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Debug/DebugLayer.h"
#include "Ayla/Window/Window.h"
#include "Ayla/Client/UserSetup.h"


using namespace Ayla::Windows;
using namespace Ayla::Events;

namespace Ayla::Core {

    class Application{
    public:
        Application();
        ~Application();

         void Run();
         void onEvent(Event&);


         static Window& getWindow();
         static LayerStack& getLayerStack();
         static Application& get();

         void setClientLoop(Client::ClientLoop* clientLoop);
    private:
        Client::ClientLoop* m_clientLoop;

        static Application* m_application;
        bool m_appIsRunning = true;
        static std::unique_ptr<Window> m_window;
        static std::unique_ptr<LayerStack> m_layerStack;

        std::unique_ptr<GUI::ImGuiLayer> m_imGuiLayer;
        std::unique_ptr<Debug::DebugLayer> m_debugLayer;

    };



} // Ayla


