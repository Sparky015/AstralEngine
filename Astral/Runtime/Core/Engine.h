/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "ApplicationModule.h"
#include "SmartPointers.h"
#include "Job/JobManager.h"
#include "Core/Time/Clock.h"


namespace Astral {
    class WindowManager;
    class SceneManager;
    class RendererManager;
    class AssetManager;
    class ImGuiManager;
}

namespace Astral {

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

       static inline Engine& Get() { return *m_Instance; }

        WindowManager& GetWindowManager() { return *m_WindowManager; }
        SceneManager& GetSceneManager() { return *m_SceneManager; }
        Astral::ImGuiManager& GetImGuiManager() { return *m_ImGuiManager; }
        RendererManager& GetRendererManager() { return *m_RendererManager; }
        AssetManager& GetAssetManager() { return *m_AssetManager; }
        JobManager& GetJobManager() { return *m_JobManager; }


       /**@brief Gets the amount of time in seconds since the engine was initialized in seconds. */
       [[nodiscard]] float GetTimeSinceInitialization() const { return m_Clock.GetTimeSeconds(); };

    private:
        static Engine* m_Instance; // Pointing to memory on the stack

        ScopedPtr<Application::ApplicationModule> m_ApplicationModule;
        bool m_IsLoopRunning;
        Clock m_Clock;
        size_t m_FrameCount;

        ScopedPtr<WindowManager> m_WindowManager;
        ScopedPtr<ImGuiManager> m_ImGuiManager;
        ScopedPtr<RendererManager> m_RendererManager;
        ScopedPtr<AssetManager> m_AssetManager;
        ScopedPtr<SceneManager> m_SceneManager;
        ScopedPtr<JobManager> m_JobManager;
    };

}
