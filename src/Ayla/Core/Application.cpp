//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"

#include "Ayla/aypch.h"



#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/ImGui/ImGuiLayer.h"




#include "glad/glad.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"



namespace Ayla::Core {

    Application* Application:: m_application = nullptr;

    Application::Application() {
        AY_ASSERT(m_application == nullptr, "Can not create more than one application!");

        // Application //
        AY_TRACE("Application: Initializing Application");
        m_application = this;

        // Window //
        AY_LOG("--- Application: Initializing Window ---");
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

        // Layer Stack //
        AY_LOG("--- Application: Initializing Layer Stack ---");
        m_layerStack = std::make_unique<LayerStack>();

        // Layers //
        AY_LOG("--- Application: Initializing Layers ---");
        m_debugLayer = std::make_unique<Debug::DebugLayer>();
        m_imGuiLayer = std::make_unique<GUI::ImGuiLayer>();

        // Systems //
        AY_LOG("--- Application: Initializing Systems ---");
        Input::InputState::get(); // initializes InputState on first call
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



    void Application::onEvent(Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; return;}
        if (event.getEventType() == MOUSE_CURSOR_MOVED) {return;}
        m_layerStack->dispatchEventBackToFront(event);
    }

    Window& Application::getWindow(){ return *m_window;}
    LayerStack& Application::getLayerStack(){return *m_layerStack;}
    Application& Application::getApplication() {
        AY_ASSERT(m_application != nullptr, "Application must be initialized before using it!");
        return *m_application;
    }

} // Ayla