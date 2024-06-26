//
// Created by sparky on 4/16/2024.
//
#pragma once

#include "Ayla/Core/Core.h"

#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Core/Layers/DebugLayer.h"
#include "Ayla/Core/Time/Time.h"

#include "Ayla/Window/Window.h"
#include "Ayla/Input/InputState.h"


using namespace Ayla::Windows;
using namespace Ayla::Events;

namespace Ayla::Core {

    class AYLA_API Application{
    public:
        Application();
        ~Application();

         void Run();
         void onEvent(Event&);

         Window& getWindow();
         LayerStack& getLayerStack();
         static Application& getApplication();

    private:

        bool m_appIsRunning = true;
        static Application* m_application;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<LayerStack> m_layerStack;

        std::unique_ptr<GUI::ImGuiLayer> m_imGuiLayer;
        std::unique_ptr<Debug::DebugLayer> m_debugLayer;
    };

    AYLA_API extern Application* CreateApplication();

} // Ayla


