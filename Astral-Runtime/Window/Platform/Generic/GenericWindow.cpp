//
// Created by Andrew Fagan on 11/29/24.
//

#include "GenericWindow.h"

#include "Window/WindowEvents.h"
#include "Core/Events/EventPublisher.h"
#include "Debug/MemoryTracking/RegionTrackingAllocators/WindowTrackingAllocators.h"
#include "Debug/Utilities/Asserts.h"
#include "Input/KeycodeConversions.h"
#include "Renderer/SceneRenderer.h"

#include "GLFW/glfw3.h"


namespace Astral {

    bool GenericWindow::m_IsGLFWInitialized = false;


    GenericWindow::GenericWindow() : m_Window(nullptr) {}


    void GenericWindow::Init()
    {
        /** Initializing GLFW */

        GLFWallocator allocator;
        allocator.allocate = WindowTrackingMalloc;
        allocator.reallocate = WindowTrackingRealloc;
        allocator.deallocate = WindowTrackingFree;
        allocator.user = NULL;

        glfwInitAllocator(&allocator);

        if (!m_IsGLFWInitialized)
        {
            if (!glfwInit())
            {
                ASTRAL_ERROR("GLFW failed to initialize!");
            }
        }

        if (Astral::SceneRenderer::GetRendererAPIBackend() == Astral::API::Vulkan)
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);


        /** Creating the GLFW window */
        m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowName.data(), nullptr, nullptr);

        if (m_Window == nullptr)
        {
            glfwTerminate();
            ASTRAL_ERROR("GLFW failed to create the window!")
        }

        SetGLFWCallbacks();
    }


    void GenericWindow::Update()
    {
        PROFILE_SCOPE("GenericWindow::Update")
        glfwPollEvents();
    }


    void GenericWindow::Shutdown()
    {
        PROFILE_SCOPE("GenericWindow::Shutdown")
        glfwTerminate();
    }


    void GenericWindow::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }


    std::string_view GenericWindow::GetVersion()
    {
        static std::string version = "GLFW " + std::string(glfwGetVersionString());
        return version;
    }


    void GenericWindow::EnableVSync()
    {
        glfwSwapInterval(1);
        m_Vsync = 1;
    }


    void GenericWindow::DisableVSync()
    {
        glfwSwapInterval(0);
        m_Vsync = 0;
    }


    void* GenericWindow::GetNativeWindow()
    {
        ASSERT(m_Window, "Window has not been created!");
        return m_Window;
    }


    void GenericWindow::SetGLFWCallbacks() const
    {
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            Astral::EventPublisher<WindowClosedEvent> windowClosedPublisher;
            windowClosedPublisher.PublishEvent(WindowClosedEvent());
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Astral::EventPublisher<KeyPressedEvent> keyPressedEvent;
                keyPressedEvent.PublishEvent(KeyPressedEvent(Input::TranslateGLFWKeycodesToAstral(key)));
            }
            else if (action == GLFW_RELEASE)
            {
                Astral::EventPublisher<KeyReleasedEvent> keyReleasedEvent;
                keyReleasedEvent.PublishEvent(KeyReleasedEvent(Input::TranslateGLFWKeycodesToAstral(key)));
            }
            else if (action == GLFW_REPEAT)
            {
                Astral::EventPublisher<KeyRepeatingEvent> keyRepeatingEvent;
                keyRepeatingEvent.PublishEvent(KeyRepeatingEvent(Input::TranslateGLFWKeycodesToAstral(key)));
            }

        });

        // Called when the cursor is moved
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            int height;
            int width;
            glfwGetWindowSize(window, &width, &height);
            Astral::EventPublisher<MouseMovedEvent> mouseMovedEvent;
            mouseMovedEvent.PublishEvent(MouseMovedEvent(xpos, height - ypos));
        });


        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(Input::TranslateGLFWKeycodesToAstral(button));
                    Astral::EventPublisher<KeyPressedEvent> keyPressedEvent;
                    keyPressedEvent.PublishEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(Input::TranslateGLFWKeycodesToAstral(button));
                    Astral::EventPublisher<KeyReleasedEvent> keyReleasedEvent;
                    keyReleasedEvent.PublishEvent(event);
                    break;
                }
            }
        });
    }


    int GenericWindow::GetWidth()
    {
        int width;
        glfwGetWindowSize(m_Window, &width, nullptr);
        return width;
    }


    int GenericWindow::GetHeight()
    {
        int height;
        glfwGetWindowSize(m_Window, nullptr, &height);
        return height;
    }


    Vec2 GenericWindow::GetFramebufferScale()
    {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return Vec2(width / m_WindowWidth, height / m_WindowHeight);
    }


    void GenericWindow::SetWindowName(std::string_view windowName)
    {
        glfwSetWindowTitle(m_Window, windowName.data());
    }


    void GenericWindow::SetWindowDimensions(int width, int height)
    {
        PROFILE_SCOPE("GenericWindow::SetWindowDimensions");
        glfwSetWindowSize(m_Window, width, height);
    }

} // Window