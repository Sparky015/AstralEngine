//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include <Ayla/aypch.h>

namespace Ayla {

    Application::Application() {
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
    }

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;


        while (!glfwWindowShouldClose(m_window->getGLFWwindow())) {
            glfwPollEvents();
        }

        exit(EXIT_SUCCESS);

    }

    void Application::onEvent(Ayla::Event &) {
        std::cout << "Event occurred!" << std::endl;
    }


} // Ayla