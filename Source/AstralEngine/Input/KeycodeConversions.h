//
// Created by Andrew Fagan on 12/6/24.
//

#pragma once

namespace Input {

    /** Converts GLFW keycodes to Sozin keycodes, so events are able use the Ayla keycodes. */
    int TranslateGLFWKeycodesToSozin(int glfwKeycode);

}