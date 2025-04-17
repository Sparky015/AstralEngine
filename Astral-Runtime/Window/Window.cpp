//
// Created by Andrew Fagan on 11/29/24.
//

#include "Window.h"

#include "Window/Platform/Generic/GenericWindow.h"

namespace Astral {

    WindowAPI Window::s_WindowAPI = WindowAPI::GLFW;

    Window* Window::CreateWindow()
    {
        switch (s_WindowAPI)
        {
            case WindowAPI::GLFW: return new GenericWindow();
            case WindowAPI::Cocoa: ASTRAL_ERROR("Cocoa Window API not supported yet!");
            case WindowAPI::Win32: ASTRAL_ERROR("Win32 Window API supported yet!");
            default: ASTRAL_ERROR("Invalid Window API");
        }
    }

} // Window