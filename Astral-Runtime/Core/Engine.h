/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "ApplicationModule.h"
#include "SmartPointers.h"
#include "Time/Clock.h"

// Forward declare the subsystem managers to avoid including large portions of the engine in the header
namespace Astral { class WindowManager; }
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

        WindowManager& GetWindowManager() { return *m_WindowManager; }
        ECSManager& GetECSManager() { return *m_ECSManager; }
        Debug::ImGuiManager& GetImGuiManager() { return *m_ImGuiManager; }

       /**@brief Gets the amount of time in seconds since the engine was initialized in seconds. */
       [[nodiscard]] float GetTimeSinceInitialization() const { return m_Clock.GetTimeSeconds(); };

    private:
        static Engine* m_Instance;

        Application::ApplicationModule* m_ApplicationModule;
        bool m_IsLoopRunning;
        Core::Clock m_Clock;

        ScopedPtr<WindowManager> m_WindowManager;
        ScopedPtr<ECSManager> m_ECSManager;
        ScopedPtr<Debug::ImGuiManager> m_ImGuiManager;
    };

}
