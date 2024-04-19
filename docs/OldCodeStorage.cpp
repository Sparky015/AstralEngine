//
// Created by Andrew Fagan on 4/18/24.
//


//#       ### SDL2 ###
//##set(SDL2_DIR ${CMAKE_SOURCE_DIR}/vendor/SDL/SDL2-windows/cmake)
//# (1/2) ^^^ Fun fact I learned while making this (for future reference), you can use 'set' to set a path to point to the SDL2Config.cmake (or sdl2-config.cmake).
//# The SDL2 files comes with a cmake configuration file SDL2Config.cmake which sets up SDL2_INCLUDE_DIRS and SDL2_LIBRARIES.
//##find_package(SDL2 HINTS ${SDL2_DIR} REQUIRED)
//# (2/2) so when you call find_package(SDL2), it searches for SDL2Config.cmake (or sdl2-config.cmake), and that file is responsible for
//# setting up all the necessary variables (like SDL2_LIBRARIES and SDL2_INCLUDE_DIRS) and targets.
//# Also, the naming of the package is based on how the cmake config file is named. It is SDL2 in this case because the config file is SDL2Config.cmake.
//# So its basically the package name is * where the config file is named *Config.cmake (or *-config.cmake).

//##target_link_libraries(Ayla ${SDL2_LIBRARIES})


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