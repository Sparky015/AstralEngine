//
// Created by Andrew Fagan on 4/18/24.
//
#include <Ayla/WindowSystem/GenericWindow.h>

namespace Ayla{



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
    }

    int GenericWindow::getWidth() const {
        return m_windowProperties.width;
    }

    int GenericWindow::getHeight() const {
        return m_windowProperties.height;;
    }

    void GenericWindow::setEventCallback(const Ayla::Window::EventCallback& callback) {
        m_callback = callback;
    }

    void GenericWindow::onUpdate() {

    }

    GLFWwindow* GenericWindow::getGLFWwindow() {
        return m_window;
    }

}