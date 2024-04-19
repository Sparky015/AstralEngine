//
// Created by sparky on 4/16/2024.
//
#pragma once
#include <Ayla/Core.h>
#include <Ayla/WindowSystem/Window.h>
#include <Ayla/WindowSystem/GenericWindow.h>
namespace Ayla {

     class AYLA_API Application {
     public:
        Application();

        ~Application();

        void Run();

     private:
         bool m_appIsRunning = true;
         std::unique_ptr<GenericWindow> m_window = std::make_unique<GenericWindow>();
    };

     // Defined Client Side
    AYLA_API extern Application* CreateApplication();

} // Ayla


