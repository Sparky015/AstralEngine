//
// Created by sparky on 4/16/2024.
//
#pragma once
#include "Ayla/Core/Core.h"
#include "Ayla/WindowSystem/Window.h"


namespace Ayla {

    class AYLA_API Application{
    public:
            Application();
            ~Application();

             void Run();
             void onEvent(Event&);

             Window& getWindow();
             static Application& getApplication();

    private:

        bool m_appIsRunning = true;
        static Application* m_application;
        std::unique_ptr<Window> m_window;
    };

    AYLA_API extern Application* CreateApplication();

} // Ayla


