//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"

#include <memory>
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/aypch.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Input/InputLayer.h"
#include "glad/glad.h"


#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

namespace Ayla {

    Application* Application:: m_application = nullptr;

    Application::Application() {
        AY_ASSERT(m_application == nullptr, "Can not create more than one application!");

        m_application = this;
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
        m_layerStack = std::make_unique<LayerStack>();

    }

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;

        initializeLayers();

        while (m_appIsRunning){

            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_layerStack->update();

            m_window->update(); // Must be called last
        }

        exit(EXIT_SUCCESS);
    }

    void Application::initializeLayers() {
        m_imGuiLayer = std::make_unique<ImGuiLayer>();
        m_inputLayer = std::make_unique<InputLayer>();
    }

    // TODO: Host the Event Dispatcher inside the application to handle in the OnEvent function!!
    void Application::onEvent(Ayla::Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; }
        if (event.getEventType() == MOUSE_CURSOR_MOVED) return;
        if (event.getEventType() == WINDOW_RESIZE){
            auto resizeEvent = dynamic_cast<Ayla::WindowResizeEvent&>(event);
            std::cout << "\n Window resized to: " << resizeEvent.getHeight() << " by " << resizeEvent.getWidth() << "\n";
        }

        std::cout << "Event occurred!" << std::endl;
    }

    Window& Application::getWindow(){ return *m_window;}
    LayerStack& Application::getLayerStack(){return *m_layerStack;}
    Application& Application::getApplication() {
        AY_ASSERT(m_application != nullptr, "Application must be initialized before getting it!");
        return *m_application;
    }

} // Ayla