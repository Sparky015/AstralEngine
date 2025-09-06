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
namespace Astral { class SceneManager; }
namespace Astral { class RendererManager; }
namespace Astral { class AssetManager; }
namespace Astral { class ImGuiManager; }

namespace Astral {

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

       static inline Engine& Get() { return *m_Instance; }

        WindowManager& GetWindowManager() { return *m_WindowManager; }
        SceneManager& GetSceneManager() { return *m_ECSManager; }
        Astral::ImGuiManager& GetImGuiManager() { return *m_ImGuiManager; }
        RendererManager& GetRendererManager() { return *m_RendererManager; }
        AssetManager& GetAssetManager() { return *m_AssetManager; }

       /**@brief Gets the amount of time in seconds since the engine was initialized in seconds. */
       [[nodiscard]] float GetTimeSinceInitialization() const { return m_Clock.GetTimeSeconds(); };

    private:
        static Engine* m_Instance; // Pointing to memory on the stack

        ScopedPtr<Application::ApplicationModule> m_ApplicationModule;
        bool m_IsLoopRunning;
        Astral::Clock m_Clock;

        ScopedPtr<WindowManager> m_WindowManager;
        ScopedPtr<Astral::ImGuiManager> m_ImGuiManager;
        ScopedPtr<RendererManager> m_RendererManager;
        ScopedPtr<AssetManager> m_AssetManager;
        ScopedPtr<SceneManager> m_ECSManager;
    };

}
