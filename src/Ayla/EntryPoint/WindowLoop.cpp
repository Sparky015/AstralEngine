//
// Created by Andrew Fagan on 4/16/24.
//
#include "Ayla/EntryPoint/WindowLoop.h"


#include <iostream>
#include <SDL.h>
#include <cstdlib>

#include <GLFW/glfw3.h>

namespace Ayla::EntryPoint {

    void WindowLoop(){
        std::cout << "Hello, World!" << std::endl;

        //SDL Implementation:
//        // Remember to initialize SDL
//        if (SDL_Init(SDL_INIT_VIDEO) < 0){
//            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        SDL_CreateWindow("test", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
//        SDL_Event event;
//        bool hasQuit = false;
//        while(!hasQuit){
//            while(SDL_PollEvent( &event )){
//                if(event.type == SDL_QUIT) { hasQuit = true; }
//            }
//        }
//
//        SDL_Quit();   // Cleanup after we're done

        // GLFW Implementation
        if (!glfwInit()) {
            std::cout << "GLFW could not initialize!" << std::endl;
            exit(EXIT_FAILURE);
        }

        GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Window", nullptr, nullptr);
        if (!window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        glfwTerminate();


        exit(EXIT_SUCCESS);
    }

} // namespace Ayla::EntryPoint