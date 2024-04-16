//
// Created by Andrew Fagan on 4/16/24.
//
#include "Ayla/EntryPoint/EngineLoop.h"


#include <iostream>
#include <SDL.h>
#include <cstdlib>


namespace Ayla::EntryPoint {

    void EngineLoop(){
        std::cout << "Hello, World!" << std::endl;

        // Remember to initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        SDL_CreateWindow("test", 100, 100, 500, 500, SDL_WINDOW_SHOWN);
        SDL_Event event;
        bool hasQuit = false;
        while(!hasQuit){
            while(SDL_PollEvent( &event )){
                if(event.type == SDL_QUIT) { hasQuit = true; }
            }
        }

        SDL_Quit();   // Cleanup after we're done

        exit(EXIT_SUCCESS);
    }

} // namespace Ayla::EntryPoint