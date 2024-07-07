//
// Created by Andrew Fagan on 7/2/24.
//

#pragma once

namespace Ayla::Input::Keycodes::Translation {

    /** Converts GLFW keycodes to Ayla keycodes, so events are able use the Ayla keycodes. */
    int TranslateGlfwKeycodesToAyla(int glfwKeycode);

}
