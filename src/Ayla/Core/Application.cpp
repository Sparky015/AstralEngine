//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include <Ayla/EventSystem/ApplicationEvent.h>
#include <Ayla/aypch.h>

namespace Ayla {

    Application::Application() {
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
    }

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;


        while (m_appIsRunning){
            m_window->onUpdate();
        }

        exit(EXIT_SUCCESS);

    }

    void Application::onEvent(Ayla::Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; }
        if (event.getEventType() == MOUSE_CURSOR_MOVED) return;
        if (event.getEventType() == WINDOW_RESIZE){
            auto resizeEvent = dynamic_cast<Ayla::WindowResizeEvent&>(event);
            std::cout << "\n Window resized to: " << resizeEvent.getHeight() << " by " << resizeEvent.getWidth() << "\n";
        }

        std::cout << "Event occurred!" << std::endl;
    }


} // Ayla