/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "ApplicationModule.h"
#include "Events/EventListener.h"
#include "Events/EventPublisher.h"
#include "SmartPointers.h"
#include "Time/Clock.h"

#include "Debug/ImGui/ImGuiEvents.h"
#include "EngineLoopEvents.h"
#include "Window/WindowEvents.h"


namespace Window { class WindowManager; }
namespace Astral { class ECSManager; }
namespace Debug { class ImGuiManager; }

namespace Astral {

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

       static inline Engine& Get() { return *m_Instance; }

        Window::WindowManager& GetWindowManager() { return *m_WindowManager; }
        Astral::ECSManager& GetECSManager() { return *m_ECSManager; }
        Debug::ImGuiManager& GetImGuiManager() { return *m_ImGuiManager; }

       /**@brief Gets the amount of time in seconds since the engine was initialized in seconds. */
       [[nodiscard]] float GetTimeSinceInitialization() const { return m_Clock.GetTimeSeconds(); };

    private:
        Core::EventPublisher<SubSystemUpdateEvent> m_SubSystemUpdatePublisher;
        Core::EventPublisher<NewFrameEvent> m_NewFramePublisher;
        Core::EventPublisher<RenderImGuiEvent> m_RenderImGuiPublisher;
        Core::EventListener<WindowClosedEvent> m_WindowClosedListener;

        static Engine* m_Instance;
        Application::ApplicationModule* m_ApplicationModule;
        bool m_IsLoopRunning;
        Core::Clock m_Clock;

        Astral::ScopedPtr<Window::WindowManager> m_WindowManager;
        Astral::ScopedPtr<Astral::ECSManager> m_ECSManager;
        Astral::ScopedPtr<Debug::ImGuiManager> m_ImGuiManager;
    };

}
