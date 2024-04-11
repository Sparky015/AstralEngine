#include <iostream>

#include <SDL.h>
int main() {
    std::cout << "Hello, World!" << std::endl;

    SDL_CreateWindow("test", 100, 100, 500, 500, SDL_WINDOW_SHOWN);
    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
    return 0;
}


