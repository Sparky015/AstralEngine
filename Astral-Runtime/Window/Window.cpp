//
// Created by Andrew Fagan on 11/29/24.
//

#include "Window.h"

#include "Debug/Utilities/Error.h"
#include "Window/Platform/Generic/GenericWindow.h"

namespace Astral {

    WindowAPI Window::s_WindowAPI = WindowAPI::GLFW;

    Window* Window::ConstructWindow()
    {
        switch (s_WindowAPI)
        {
            case WindowAPI::GLFW: return new GenericWindow();
            case WindowAPI::Cocoa: AE_ERROR("Cocoa Window API not supported yet!");
            case WindowAPI::Win32: AE_ERROR("Win32 Window API supported yet!");
            default: AE_ERROR("Invalid Window API");
        }
    }

} // Window