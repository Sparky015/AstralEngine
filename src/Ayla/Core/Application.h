//
// Created by sparky on 4/16/2024.
//
#pragma once
#include "Ayla/Core/Core.h"
#include "Ayla/Window/Window.h"
#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Input/InputState.h"

namespace Ayla {

    class AYLA_API Application{
    public:
            Application();
            ~Application();

             void Run();
             void onEvent(Ayla::Event&);

             Window& getWindow();
             LayerStack& getLayerStack();
             static Application& getApplication();

    private:

        bool m_appIsRunning = true;
        static Application* m_application;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<LayerStack> m_layerStack;

        std::unique_ptr<ImGuiLayer> m_imGuiLayer;

    };

    AYLA_API extern Application* CreateApplication();

} // Ayla


