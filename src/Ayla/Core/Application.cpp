//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/aypch.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "glad/glad.h"


#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

namespace Ayla {

    Application* Application:: m_application = nullptr;

    Application::Application() {
        AY_ASSERT(m_application == nullptr, "Only one application should be created!");

        m_application = this;
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
    }

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;

        ImGuiLayer testLayer = ImGuiLayer();
        while (m_appIsRunning){

            glClearColor(0, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            LayerStack::get().update();

            m_window->update(); // Must be called last
        }
        testLayer.detachLayer();
        exit(EXIT_SUCCESS);

    }

    void Application::onEvent(Ayla::Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; }
        if (event.getEventType() == MOUSE_CURSOR_MOVED) return;
        if (event.getEventType() == WINDOW_RESIZE){
            auto resizeEvent = dynamic_cast<Ayla::WindowResizeEvent&>(event);
            std::cout << "\n Window resized to: " << resizeEvent.getHeight() << " by " << resizeEvent.getWidth() << "\n";
        }

        std::cout << "Events occurred!" << std::endl;
    }

    Window& Application::getWindow(){ return *m_window;}

    Application& Application::getApplication() {
        return *m_application;
    }

} // Ayla