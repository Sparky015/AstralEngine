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
        AY_TRACE("Application: Initializing Application");
        m_application = this;
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

        m_layerStack = std::make_unique<LayerStack>();
        m_imGuiLayer = std::make_unique<ImGuiLayer>();
        AY_TRACE("Application: Between the ImGui init and InputState init");
        InputState::get();
    }

    Application::~Application() = default;

    void Application::Run() {
        std::cout << "\n\nRunning Application!" << std::endl;

        while (m_appIsRunning){

            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_layerStack->update();

            m_window->update(); // Must be called last
        }

        exit(EXIT_SUCCESS);
    }



    void Application::onEvent(Ayla::Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; }

        m_layerStack->dispatchEventBackToFront(event);

    }

    Window& Application::getWindow(){ return *m_window;}
    LayerStack& Application::getLayerStack(){return *m_layerStack;}
    Application& Application::getApplication() {
        AY_ASSERT(m_application != nullptr, "Application must be initialized before using it!");
        return *m_application;
    }

} // Ayla