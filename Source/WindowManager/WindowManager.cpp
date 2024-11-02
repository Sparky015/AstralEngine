//
// Created by Andrew Fagan on 10/31/24.
//

#include "WindowManager.h"

#include "WindowEvents.h"
#include "Core/Events/EventPublisher.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Window{

    WindowManager& g_WindowManager = WindowManager::Get();

    void WindowManager::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    WindowManager::WindowManager() : m_Window{nullptr}
    {
        LOG("Window Created!")
    }

    WindowManager::~WindowManager()
    {
        LOG("Window Destroyed!")
    }


    WindowManager& WindowManager::Get()
    {
        static WindowManager m_Instance = WindowManager();
        return m_Instance;
    }


    bool WindowManager::m_IsGLFWInitialized = false;

    void WindowManager::Init()
    {


        ///  Initializing GLFW

        if (!m_IsGLFWInitialized)
        {
            if (!glfwInit())
            {
                ERROR("GLFW failed to initialize!");
            }
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

        m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Chess", nullptr, nullptr);

        if (m_Window == nullptr)
        {
            glfwTerminate();
            ERROR("GLFW failed to create the window!")
        }

        glfwMakeContextCurrent(m_Window);

        int const status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status != 1)
        {
            ERROR("GLAD failed to load!");
        }

        /// Setting GLFW callbacks

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            Event::EventPublisher<WindowClosedEvent> windowClosedPublisher;
            windowClosedPublisher.PublishEvent(WindowClosedEvent());
        });
    }

    void WindowManager::Shutdown()
    {
        glfwTerminate();
    }

}
