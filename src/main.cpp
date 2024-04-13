#include <iostream>

#include "SDL.h"

// SDL REQUIRES that it has the 'int argc, char* args[]' main signature, or it will cry.
int main( int argc, char* args[] ) {
    std::cout << "Hello, World!" << std::endl;

    // Remember to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_CreateWindow("test", 100, 100, 500, 500, SDL_WINDOW_SHOWN);
    SDL_Event e;
    bool quit = false;
    while(!quit){
        while(SDL_PollEvent( &e )){
            if(e.type == SDL_QUIT) quit = true;
        }
    }

    SDL_Quit();   // Cleanup after we're done

    return 0;
}


