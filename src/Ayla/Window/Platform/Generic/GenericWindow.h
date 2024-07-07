//
// Created by Andrew Fagan on 4/18/24.
//
#pragma once
#include "Ayla/Events/Event.h"
#include "Ayla/Window/Window.h"
#include "Ayla/aypch.h"

#include "GLFW/glfw3.h"

namespace Ayla::Windows{


    static void GlfwErrorCallback(int error, const char* description);

    class GenericWindow : public Window {
    public:

        explicit GenericWindow(const WindowProperties& properties);
        ~GenericWindow() override;

        void Init(const WindowProperties &windowProperties);
        [[nodiscard]] int GetWidth() const override;
        [[nodiscard]] int GetHeight() const override;
        [[nodiscard]] float GetDisplayFramebufferScaleX() const override;
        [[nodiscard]] float GetDisplayFramebufferScaleY() const override;
        [[nodiscard]] void* GetNativeWindow() const override;
        virtual void SetEventCallback(const EventCallback& callback) override;
        void Update() override;

    private:
        GLFWwindow* m_Window;
        WindowProperties m_WindowProperties;

        struct WindowData {
            int Width;
            int Height;
            float DisplayFramebufferScaleX;
            float DisplayFramebufferScaleY;
            std::string Title;
            bool IsVSyncEnabled;

            EventCallback Callback;
        };

        WindowData m_WindowData;
        static bool m_IsGlfwInitialized;
    };
} // namespace Ayla::Windows
