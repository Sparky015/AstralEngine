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

        /**
         * @brief Initializes the engine and all subsystems
         */
        void Init();

        /**
         * @brief Starts the engine run loop
         */
        void Run();

        /**
         * @brief Shutdowns the engine and all subsystems
         */
        void Shutdown();

        /**
         * @brief Gets the engine singleton instance
         * @return The engine singleton instance
         */
       static inline Engine& Get() { return *m_Instance; }

        /**
         * @brief Gets the window manager
         * @note The engine instance owns the lifetime of the window manager
         */
        WindowManager& GetWindowManager() { return *m_WindowManager; }

        /**
         * @brief Gets the scene manager
         * @note The engine instance owns the lifetime of the scene manager
         */
        SceneManager& GetSceneManager() { return *m_SceneManager; }

        /**
         * @brief Gets the imgui manager
         * @note The engine instance owns the lifetime of the imgui manager
         */
        Astral::ImGuiManager& GetImGuiManager() { return *m_ImGuiManager; }

        /**
         * @brief Gets the renderer manager
         * @note The engine instance owns the lifetime of the renderer manager
         */
        RendererManager& GetRendererManager() { return *m_RendererManager; }

        /**
         * @brief Gets the asset manager
         * @note The engine instance owns the lifetime of the asset manager
         */
        AssetManager& GetAssetManager() { return *m_AssetManager; }

        /**
         * @brief Gets the job manager
         * @note The engine instance owns the lifetime of the job manager
         */
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
