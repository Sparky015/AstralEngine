//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Window.h"

#include <memory>

namespace Astral {

    class WindowManager : public SystemManager
    {
    public:
        explicit WindowManager();
        ~WindowManager() override;

        static WindowManager& Get();

        void Init() override;
        void Shutdown() override;
        void Update() override;

        [[nodiscard]] inline Window& GetWindow() const { return *m_Window; };
        inline void SwapBuffers() const { m_Window->SwapBuffers(); }


        WindowManager(WindowManager&&) = delete;
        WindowManager& operator=(WindowManager&&) = delete;
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

    private:

        std::unique_ptr<Window> m_Window;
    };

}
