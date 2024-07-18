//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include "Ayla/Core/Layers/LayerStack.h"
#include "Ayla/Core/Time/Clock.h"
#include "Ayla/Debug/DebugLayer.h"
#include "Ayla/Debug/LogTools.h"
#include "Ayla/ImGui/ImGuiLayer.h"
#include "Ayla/Input/InputState.h"

#include "Ayla/Renderer/RendererLayer.h" // TEMP


namespace Ayla::Core
{
    Application* Application::m_Application = nullptr;
    std::unique_ptr<LayerStack> Application::m_LayerStack = nullptr;
    std::unique_ptr<Window> Application::m_Window = nullptr;


    Application::Application() : m_ClientLoop(nullptr)
    {
        AY_ASSERT(m_Application == nullptr, "[Sholas] Core/Application.cpp: Can not create more than one application!");

        // Application //
        AY_TRACE("[Sholas] Application: Initializing Application");
        m_Application = this;

        // Window //
        AY_LOG("--- Application: Initializing Window ---");
        m_Window = std::unique_ptr<Window>(Window::CreateWindow(Windows::WindowProperties("Ayla Engine Window", 1024, 768)));
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        // Layer Stack //
        AY_LOG("--- Application: Initializing Layer Stack ---");
        m_LayerStack = std::make_unique<LayerStack>();

        // Layers //
        AY_LOG("--- Application: Initializing Layers ---"); // Order matters here. This will reflect the order of the layers in LayerStack.
        m_ImGuiLayer = std::make_unique<GUI::ImGuiLayer>();
        Input::SInputState::Init(); // Initializes Input layer on the first call.
        m_DebugLayer = std::make_unique<Debug::DebugLayer>();

        // Clock //
        Time::Clock::Init();

        Ayla::Debug::LogTools(); // Log all the systems being used after initialization

        static Renderer::RendererLayer rendererLayer = Renderer::RendererLayer(); // TEMP
    }


    Application::~Application()
    {
        AY_TRACE("[Sholas] Application: Destroying Application");
        delete m_ClientLoop;
        Input::SInputState::Destroy();
        Time::Clock::Destroy();
    };


    void Application::Run() const
    {
        AY_PROFILE_SCOPE("Application Run Loop");

        AY_LOG("\n\nRunning Application!");

        while (m_IsAppRunning)
        {
            Time::Clock::Get().UpdateDeltaTime(); // Phase 1  -> Updates the clock.
            Time::Clock::Get().CheckTimers();

            // TODO: Provide the delta time to the update function
            m_ClientLoop->Update(); // Phase 2  -> Calls the client's application update function.

            m_LayerStack->Update(); // Phase 3  -> Calls the update function on all the layers in the engine.

            m_ImGuiLayer->Begin(); // Phase 4  -> Updates the ImGui UI for all the layers in the engine.
            m_LayerStack->RenderImGui();
            m_ImGuiLayer->End();

            m_Window->Update(); // Phase 5  ->  Polls the Window events and swaps the buffer. Must be called last.
        }

        AY_LOG_SS("\nStopwatch: " << static_cast<double>(Time::Clock::Get().GetStopwatchTime()) * .001 << " seconds\n");
    }


    void Application::OnEvent(IEvent& event) // TODO: Make the parameter const and the depending functions const.
    {
        if (event.GetEventType() == Events::WINDOW_CLOSE)
        {
            m_IsAppRunning = false;
            return;
        }
        // TODO: Change the naming of this method. Its confusing.
        m_LayerStack->DispatchEventBackToFront(event);
    }


    Window& Application::GetWindow()
    {
        return *m_Window;
    }


    LayerStack& Application::GetLayerStack()
    {
        return *m_LayerStack;
    }


    Application& Application::Get()
    {
        AY_ASSERT(m_Application != nullptr, "[Sholas] Core/Application.cpp: Application must be initialized before using it!");
        return *m_Application;
    }


    void Application::SetClientLoop(Client::ClientLoop* clientLoop)
    {
        AY_ASSERT(m_ClientLoop == nullptr, "[Sholas] Core/Application.cpp: ClientLoop has already been initialized!");
        m_ClientLoop = clientLoop;
    }


} // namespace Ayla::Core