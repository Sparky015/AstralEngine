//
// Created by sparky on 4/16/2024.
//

#include <Ayla/Application.h>
#include <Ayla/EntryPoint/WindowLoop.h>

namespace Ayla {

    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::Run() {
        EntryPoint::WindowLoop();
    }

} // Ayla