//
// Created by Andrew Fagan on 4/18/24.
//
#include "GenericWindow.h"


#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/MouseEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Input/KeycodeTranslation/GLFWTranslation.h"

#include "glad/glad.h"
#include "imgui.h"



namespace Ayla::Windows {

    // PUBLIC
    void GLFWErrorCallback(int error, const char* description){
        std::cout << description << error;
    }

    GenericWindow::GenericWindow(const WindowProperties& properties){
        AY_TRACE("[Sholas] GenericWindow: Initializing Generic Window");
        init(properties);

    }

    GenericWindow::~GenericWindow(){
        AY_TRACE("[Sholas] GenericWindow: Destroying Generic Window");
        glfwTerminate();
    }

    Window* Window::createWindow(const WindowProperties& properties) {
        return new GenericWindow(properties);
    }

    void GenericWindow::init(const WindowProperties& windowProperties) {
        m_windowData.title = windowProperties.title;
        m_windowData.height = windowProperties.height;
        m_windowData.width = windowProperties.width;


        // GLFW Implementation
        if (!s_isGLFWInitialized){
            if (!glfwInit()) {
                std::cout << "GLFW could not initialize!" << std::endl;
                exit(EXIT_FAILURE);
            }
            s_isGLFWInitialized = true;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

        m_window = glfwCreateWindow(windowProperties.width, windowProperties.height, windowProperties.title.c_str(), nullptr, nullptr);
        if (!m_window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }


        glfwMakeContextCurrent(m_window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status != 1) { std::cout << "GLAD failed to load!"; }
        glfwSetWindowUserPointer(m_window, &m_windowData);

        // Initializes the DisplayFramebufferScale
        int framebufferWidth;
        int framebufferHeight;
        glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
        m_windowData.displayFramebufferScaleX = framebufferWidth > 0 ? ((float)framebufferWidth / (float)m_windowData.width) : 0;
        m_windowData.displayFramebufferScaleY = framebufferHeight > 0 ? ((float)framebufferHeight / (float)m_windowData.height) : 0;


                    // Setting Callbacks //

        // Called when window is resized
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
            WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            windowData.width = width;
            windowData.height = height;

            WindowResizeEvent event(width, height);
            windowData.callback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            windowData.callback(event);
        });

        glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focus){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            if (focus){
                WindowGainedFocusEvent event;
                windowData.callback(event);
            } else {
                WindowLostFocusEvent event;
                windowData.callback(event);
            }
        });

        // Called when window is moved
        glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xpos, int ypos){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            WindowMovedEvent event(xpos, ypos);
            windowData.callback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
            using Ayla::Input::Translation::translateGLFWKeycodesToAyla;

            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            switch (action){
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(translateGLFWKeycodesToAyla(button));
                    windowData.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent event(translateGLFWKeycodesToAyla(button));
                    windowData.callback(event);
                    break;
                }
                default:
                    break;
            }
            //windowData.callback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            using Ayla::Input::Translation::translateGLFWKeycodesToAyla;

            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            switch (action){
                case GLFW_PRESS:{
                    KeyPressedEvent event(translateGLFWKeycodesToAyla(key));
                    windowData.callback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    KeyReleasedEvent event(translateGLFWKeycodesToAyla(key));
                    windowData.callback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    KeyPressedRepeatingEvent event(translateGLFWKeycodesToAyla(key));
                    windowData.callback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            windowData.callback(event);
        });

        // Called when the cursor is moved
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            MouseMovedEvent event(xpos, ypos);
            windowData.callback(event);
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            MouseScrollEvent event(xoffset, yoffset);
            windowData.callback(event);
        });

        glfwSetFramebufferSizeCallback(m_window,[](GLFWwindow* window, int width, int height){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            windowData.displayFramebufferScaleX = width > 0 ? ((float)width / (float)windowData.width) : 0;
            windowData.displayFramebufferScaleY = height > 0 ? ((float)height / (float)windowData.height) : 0;
        });


        glfwSetErrorCallback(GLFWErrorCallback);
    }

    int GenericWindow::getWidth() const {
        return m_windowData.width;
    }

    int GenericWindow::getHeight() const {
        return m_windowData.height;;
    }

    float GenericWindow::getDisplayFramebufferScaleX() const {
        return m_windowData.displayFramebufferScaleX;
    }

    float GenericWindow::getDisplayFramebufferScaleY() const {
        return m_windowData.displayFramebufferScaleY;
    }

    void GenericWindow::setEventCallback(const Window::EventCallback& callback) {
        m_windowData.callback = callback;
    }

    void GenericWindow::update() {

        glfwPollEvents();
        glfwSwapBuffers(m_window);

        int display_w, display_h;
        double mouse_w, mouse_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glfwGetCursorPos(m_window, &mouse_w, &mouse_h);

        m_windowData.displayFramebufferScaleX = display_w > 0 ? ((float)display_w / (float)m_windowData.width) : 0;
        m_windowData.displayFramebufferScaleY = display_h > 0 ? ((float)display_h / (float)m_windowData.height) : 0;
    }

    void* GenericWindow::getNativeWindow() const {
        return m_window;
    }

    // PRIVATE


}