//
// Created by sparky on 4/16/2024.
//

#include <Ayla/Application.h>
#include <Ayla/WindowSystem/GenericWindow.h>
#include <Ayla/aypch.h>

namespace Ayla {

    Application::Application() = default;

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;


        while (!glfwWindowShouldClose(m_window->getGLFWwindow())) {
            glfwPollEvents();
        }

        exit(EXIT_SUCCESS);

    }



} // Ayla