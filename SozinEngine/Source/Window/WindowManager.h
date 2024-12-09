//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once

#include "Core/SystemManager.h"

#include "GLFW/glfw3.h"
#include "Window.h"


namespace Window {

    class WindowManager : public SystemManager
    {
    public:

        static WindowManager& Get();

        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline int GetWidth() const {return m_WindowWidth;}
        [[nodiscard]] inline int GetHeight() const {return m_WindowHeight;}

        [[nodiscard]] inline Window& GetWindow() const { return *m_Window; };
        inline void SwapBuffers() const { m_Window->SwapBuffers(); }


        WindowManager(WindowManager&&) = delete;
        WindowManager& operator=(WindowManager&&) = delete;
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

    private:
        WindowManager();
        ~WindowManager() override;

        std::unique_ptr<Window> m_Window;

        int m_WindowWidth = 800;
        int m_WindowHeight = 800;

    };

    extern WindowManager& g_WindowManager;
}
