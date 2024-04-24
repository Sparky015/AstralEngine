//
// Created by Andrew Fagan on 4/18/24.
//
#include "Ayla/WindowSystem/GenericWindow.h"
#include <Ayla/EventSystem/ApplicationEvent.h>
#include <Ayla/EventSystem/MouseEvent.h>
#include <Ayla/EventSystem/KeyEvent.h>
namespace Ayla{

    // PUBLIC

    GenericWindow::GenericWindow(const WindowProperties& properties){
        init(properties);
    }

    GenericWindow::~GenericWindow(){
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

        m_window = glfwCreateWindow(windowProperties.width, windowProperties.height, windowProperties.title.c_str(), nullptr, nullptr);
        if (!m_window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwSetWindowUserPointer(m_window, &m_windowData);


        // Called when window is resized
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
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
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            switch (action){
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(button);
                    windowData.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent event(button);
                    windowData.callback(event);
                    break;
                }
                default:
                    break;
            }
            //windowData.callback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            WindowData& windowData = *(WindowData*) glfwGetWindowUserPointer(window);
            switch (action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key);
                    windowData.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    windowData.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedRepeatingEvent event(key);
                    windowData.callback(event);
                    break;
                }
                default:
                    break;
            }
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


    }

    int GenericWindow::getWidth() const {
        return m_windowProperties.width;
    }

    int GenericWindow::getHeight() const {
        return m_windowProperties.height;;
    }

    void GenericWindow::setEventCallback(const Ayla::Window::EventCallback& callback) {
        m_windowData.callback = callback;
    }

    void GenericWindow::onUpdate() {
        glfwPollEvents();
    }

    GLFWwindow* GenericWindow::getGLFWwindow() {
        return m_window;
    }
    // PRIVATE


}