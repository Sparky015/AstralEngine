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

        // Clock //
        Time::Clock::get();

        // Window //
        AY_LOG("--- Application: Initializing Window ---");
        m_window = std::unique_ptr<Window>(Window::createWindow());
        m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

        // Layer Stack //
        AY_LOG("--- Application: Initializing Layer Stack ---");
        m_layerStack = std::make_unique<LayerStack>();

        // Layers //
        AY_LOG("--- Application: Initializing Layers ---");
        m_imGuiLayer = std::make_unique<GUI::ImGuiLayer>();
        Input::InputState::get(); // initializes Input layer and InputState on first call
        m_debugLayer = std::make_unique<Debug::DebugLayer>();


    }

    Application::~Application() = default;

    void Application::Run() {
        long long timeAccumulation = 0.0;
        std::cout << "\n\nRunning Application!" << std::endl;

        while (m_appIsRunning){

            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            Time::Clock::get().updateDeltaTime();
            timeAccumulation += Time::Clock::get().getDeltaTime();
            m_layerStack->update();

            m_window->update(); // Must be called last
        }
        std::cout << "\nDelta Time Accumulation: " << timeAccumulation * .001 << " seconds\n";
        std::cout << "Stopwatch: " << Time::Clock::get().getStopwatchTime() * .001 << " seconds\n";
        exit(EXIT_SUCCESS);
    }



    void Application::onEvent(Event& event) {
        if (event.getEventType() == WINDOW_CLOSE) { m_appIsRunning = false; return;}

        m_layerStack->dispatchEventBackToFront(event);
    }

    Window& Application::getWindow(){ return *m_window;}
    LayerStack& Application::getLayerStack(){return *m_layerStack;}
    Application& Application::getApplication() {
        AY_ASSERT(m_application != nullptr, "Application must be initialized before using it!");
        return *m_application;
    }

} // Ayla