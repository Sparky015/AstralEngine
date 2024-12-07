//
// Created by Andrew Fagan on 11/29/24.
//

#include "GenericWindow.h"

#include "Window/WindowEvents.h"
#include "Core/Events/EventPublisher.h"
#include "Input/KeycodeConversions.h"

namespace Window {

    bool GenericWindow::m_IsGLFWInitialized = false;


    GenericWindow::GenericWindow() : m_Window(nullptr) {}


    void GenericWindow::Init()
    {
        /** Initializing GLFW */

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


        /** Creating the GLFW window */
        m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Chess", nullptr, nullptr);

        if (m_Window == nullptr)
        {
            glfwTerminate();
            ERROR("GLFW failed to create the window!")
        }

        m_RenderContext.reset(Renderer::RendererContext::CreateRendererContext());
        m_RenderContext->Init();

        SetGLFWCallbacks();
    }


    void GenericWindow::Update()
    {
        glfwPollEvents();
    }


    void GenericWindow::Shutdown()
    {
        m_RenderContext->Shutdown();
        glfwTerminate();
    }


    void GenericWindow::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }


    std::string GenericWindow::GetVersion()
    {
        std::string version("GLFW ");
        version += glfwGetVersionString();
        return version;
    }


    void GenericWindow::EnableVsync()
    {
        glfwSwapInterval(1);
    }


    void GenericWindow::DisableVsync()
    {
        glfwSwapInterval(0);
    }


    Renderer::RendererContext& GenericWindow::GetRendererContext()
    {
        ASSERT(m_RenderContext, "Renderer context has not been created!");
        return *m_RenderContext;
    }


    void* GenericWindow::GetNativeWindow()
    {
        ASSERT(m_Window, "Window has not been created!");
        return m_Window;
    }


    void GenericWindow::SetGLFWCallbacks()
    {
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            Event::EventPublisher<WindowClosedEvent> windowClosedPublisher;
            windowClosedPublisher.PublishEvent(WindowClosedEvent());
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Event::EventPublisher<KeyPressedEvent> keyPressedEvent;
                keyPressedEvent.PublishEvent(KeyPressedEvent(Input::TranslateGLFWKeycodesToSozin(key)));
            }
            else if (action == GLFW_RELEASE)
            {
                Event::EventPublisher<KeyReleasedEvent> keyReleasedEvent;
                keyReleasedEvent.PublishEvent(KeyReleasedEvent(Input::TranslateGLFWKeycodesToSozin(key)));
            }
            else if (action == GLFW_REPEAT)
            {
                Event::EventPublisher<KeyRepeatingEvent> keyRepeatingEvent;
                keyRepeatingEvent.PublishEvent(KeyRepeatingEvent(Input::TranslateGLFWKeycodesToSozin(key)));
            }

        });
    }

} // Window