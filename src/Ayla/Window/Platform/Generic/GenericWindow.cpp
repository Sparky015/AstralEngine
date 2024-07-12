//
// Created by Andrew Fagan on 4/18/24.
//
#include "GenericWindow.h"

#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Events/MouseEvent.h"
#include "Ayla/Input/KeycodeTranslation/GLFWTranslation.h"

#include "glad/glad.h"
#include "imgui.h"



namespace Ayla::Windows {

    void GlfwErrorCallback(int error, const char* description)
    {
        AY_ERROR(description << error);
    }


    GenericWindow::GenericWindow(const WindowProperties& properties) : m_Window(), m_WindowProperties(properties)
    {
        AY_TRACE("[Sholas] GenericWindow: Initializing Generic Window");
        Init(properties);
    }


    GenericWindow::~GenericWindow()
    {
        AY_TRACE("[Sholas] GenericWindow: Destroying Generic Window");
        glfwTerminate();
    }


    Window* Window::CreateWindow(const WindowProperties& properties)
    {
        return new GenericWindow(properties);
    }


    bool GenericWindow::m_IsGlfwInitialized = false;
    void GenericWindow::Init(const WindowProperties& windowProperties)
    {
        m_WindowData.Title = windowProperties.Title;
        m_WindowData.Height = windowProperties.Height;
        m_WindowData.Width = windowProperties.Width;


        // GLFW Implementation
        if (!m_IsGlfwInitialized)
        {
            if (!glfwInit())
            {
                AY_ERROR("GLFW could not initialize!");
            }
            m_IsGlfwInitialized = true;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

        m_Window = glfwCreateWindow(windowProperties.Width, windowProperties.Height, windowProperties.Title.c_str(),
                                    nullptr, nullptr);
        if (m_Window == nullptr)
        {
            glfwTerminate();
            AY_ERROR("Failed to create GLFW window!");
        }

        glfwMakeContextCurrent(m_Window);
        int const status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status != 1)
        {
            AY_LOG("GLAD failed to load!");
        }
        glfwSetWindowUserPointer(m_Window, &m_WindowData);

        // Initializes the DisplayFramebufferScale
        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(m_Window, &framebufferWidth, &framebufferHeight);
        m_WindowData.DisplayFramebufferScaleX = framebufferWidth > 0 ? (static_cast<float>(framebufferWidth) / static_cast<float>(m_WindowData.Width)) : 0;
        m_WindowData.DisplayFramebufferScaleY = framebufferHeight > 0 ? (static_cast<float>(framebufferHeight) / static_cast<float>(m_WindowData.Height)) : 0;

        using namespace Events;
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            windowData.Width = width;
            windowData.Height = height;

            WindowResizeEvent event(width, height);
            windowData.Callback(event);
        });


        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            windowData.Callback(event);
        });


        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int isFocused)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (isFocused)
            {
                WindowGainedFocusEvent event;
                windowData.Callback(event);
            }
            else
            {
                WindowLostFocusEvent event;
                windowData.Callback(event);
            }
        });


        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowMovedEvent event(xpos, ypos);
            windowData.Callback(event);
        });


        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            using Ayla::Input::Keycodes::Translation::TranslateGlfwKeycodesToAyla;

            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(TranslateGlfwKeycodesToAyla(button));
                    windowData.Callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent event(TranslateGlfwKeycodesToAyla(button));
                    windowData.Callback(event);
                    break;
                }
                default:
                    break;
            }
        });


        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            using Ayla::Input::Keycodes::Translation::TranslateGlfwKeycodesToAyla;

            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(TranslateGlfwKeycodesToAyla(key));
                    windowData.Callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(TranslateGlfwKeycodesToAyla(key));
                    windowData.Callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedRepeatingEvent event(TranslateGlfwKeycodesToAyla(key));
                    windowData.Callback(event);
                    break;
                }
                default:
                    break;
            }
        });


        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            windowData.Callback(event);
        });


        // Called when the cursor is moved
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(xpos, ypos);
            windowData.Callback(event);
        });


        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrollEvent event(xoffset, yoffset);
            windowData.Callback(event);
        });


        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            windowData.DisplayFramebufferScaleX = width > 0 ? ((float) width / (float) windowData.Width) : 0;
            windowData.DisplayFramebufferScaleY = height > 0 ? ((float) height / (float) windowData.Height) : 0;
        });

        glfwSetErrorCallback(GlfwErrorCallback);
    }


    int GenericWindow::GetWidth() const
    {
        return m_WindowData.Width;
    }


    int GenericWindow::GetHeight() const
    {
        return m_WindowData.Height;
    }


    float GenericWindow::GetDisplayFramebufferScaleX() const
    {
        return m_WindowData.DisplayFramebufferScaleX;
    }


    float GenericWindow::GetDisplayFramebufferScaleY() const
    {
        return m_WindowData.DisplayFramebufferScaleY;
    }


    void GenericWindow::SetEventCallback(const EventCallback& callback)
    {
        m_WindowData.Callback = callback;
    }


    void GenericWindow::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);

        // TEMP
        int displayW = 0;
        int displayH = 0;
        glfwGetFramebufferSize(m_Window, &displayW, &displayH);
        m_WindowData.DisplayFramebufferScaleX = displayW > 0 ? (static_cast<float>(displayW) / static_cast<float>(m_WindowData.Width)) : 0;
        m_WindowData.DisplayFramebufferScaleY = displayH > 0 ? (static_cast<float>(displayH) / static_cast<float>(m_WindowData.Height)) : 0;
    }


    void* GenericWindow::GetNativeWindow() const
    {
        return m_Window;
    }

} // namespace Ayla::Windows